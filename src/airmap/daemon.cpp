#include <airmap/daemon.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace uuids = boost::uuids;

namespace {
constexpr const char* component{"airmap::Daemon"};
}  // namespace

std::shared_ptr<airmap::Daemon> airmap::Daemon::create(const Configuration& configuration) {
  return std::shared_ptr<Daemon> {
    new Daemon {
      configuration
    }
  }
  ->finalize();
}

airmap::Daemon::Daemon(const Configuration& configuration)
    : configuration_{configuration}, log_{configuration_.logger} {
}

std::shared_ptr<airmap::Daemon> airmap::Daemon::finalize() {
  auto sp                  = shared_from_this();
  vehicle_tracker_monitor_ = std::make_shared<mavlink::LoggingVehicleTrackerMonitor>(component, log_.logger(), sp);

  vehicle_tracker_.register_monitor(vehicle_tracker_monitor_);

  return sp;
}

airmap::Daemon::~Daemon() {
  configuration_.channel->stop();
  configuration_.channel->unsubscribe(std::move(mavlink_channel_subscription_));
  vehicle_tracker_.unregister_monitor(shared_from_this());
}

void airmap::Daemon::start() {
  mavlink_channel_subscription_ = configuration_.channel->subscribe([sp = shared_from_this()](
      const mavlink_message_t& msg) { sp->handle_mavlink_message(msg); });
  configuration_.channel->start();
}

void airmap::Daemon::handle_mavlink_message(const mavlink_message_t& msg) {
  vehicle_tracker_.update(msg);
}

void airmap::Daemon::on_vehicle_added(const std::shared_ptr<mavlink::Vehicle>& vehicle) {
  auto submitter = TelemetrySubmitter::create(configuration_.credentials, configuration_.aircraft_id, log_.logger(),
                                              configuration_.client);
  vehicle->register_monitor(std::make_shared<mavlink::LoggingVehicleMonitor>(
      component, log_.logger(), std::make_shared<SubmittingVehicleMonitor>(submitter)));
}

void airmap::Daemon::on_vehicle_removed(const std::shared_ptr<mavlink::Vehicle>&) {
  // empty on purpose
}

std::shared_ptr<airmap::Daemon::TelemetrySubmitter> airmap::Daemon::TelemetrySubmitter::create(
    const Credentials& credentials, const std::string& aircraft_id, const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<Client>& client) {
  return std::shared_ptr<airmap::Daemon::TelemetrySubmitter>{
      new airmap::Daemon::TelemetrySubmitter{credentials, aircraft_id, logger, client}};
}

airmap::Daemon::TelemetrySubmitter::TelemetrySubmitter(const Credentials& credentials, const std::string& aircraft_id,
                                                       const std::shared_ptr<Logger>& logger,
                                                       const std::shared_ptr<Client>& client)
    : log_{logger}, client_{client}, credentials_{credentials}, aircraft_id_{aircraft_id} {
}

void airmap::Daemon::TelemetrySubmitter::activate() {
  state_ = State::active;
  request_authorization();
}

void airmap::Daemon::TelemetrySubmitter::deactivate() {
  if (state_ == State::inactive)
    return;

  state_ = State::inactive;

  if (authorization_ && flight_) {
    Flights::EndFlight::Parameters parameters;
    parameters.authorization = authorization_.get();
    parameters.id            = flight_.get().id;

    client_->flights().end_flight(parameters, [sp = shared_from_this()](const auto& result) {
      if (!result) {
        try {
          std::rethrow_exception(result.error());
        } catch (const std::exception& e) {
          sp->log_.errorf(component, "failed to end flight: %s", e.what());
        } catch (...) {
          sp->log_.errorf(component, "failed to end flight");
        }
      }
    });
  }

  authorization_requested_      = false;
  create_flight_requested_      = false;
  start_flight_comms_requested_ = false;

  authorization_.reset();
  flight_.reset();
  encryption_key_.reset();
}

void airmap::Daemon::TelemetrySubmitter::submit(const mavlink::GlobalPositionInt& position) {
  current_position_ = position;

  if (state_ == State::inactive)
    return;
  if (!authorization_ || !flight_ || !encryption_key_) {
    request_authorization();
    return;
  }

  client_->telemetry().submit_updates(flight_.get(), encryption_key_.get(),
                                      {Telemetry::Update{Telemetry::Position{
                                          milliseconds_since_epoch(Clock::universal_time()), position.lat / 1E7,
                                          position.lon / 1E7, position.alt / 1E3, position.relative_alt / 1E3, 2.}}});
}

void airmap::Daemon::TelemetrySubmitter::request_authorization() {
  if (authorization_) {
    handle_request_authorization_finished(authorization_.get());
    return;
  }

  if (authorization_requested_)
    return;

  authorization_requested_ = true;

  if (credentials_.oauth) {
    Authenticator::AuthenticateWithPassword::Params params;
    params.oauth = credentials_.oauth.get();
    client_->authenticator().authenticate_with_password(params, [sp = shared_from_this()](const auto& result) {
      if (result) {
        sp->handle_request_authorization_finished(result.value().id);
      } else {
        sp->authorization_requested_ = false;
        try {
          std::rethrow_exception(result.error());
        } catch (const std::exception& e) {
          sp->log_.errorf(component, "failed to authenticate with AirMap services: %s", e.what());
        } catch (...) {
          sp->log_.errorf(component, "failed to authenticate with AirMap services");
        }
      }
    });
  } else {
    Authenticator::AuthenticateAnonymously::Params params{uuids::to_string(uuids::random_generator()())};
    client_->authenticator().authenticate_anonymously(params, [sp = shared_from_this()](const auto& result) {
      if (result) {
        sp->handle_request_authorization_finished(result.value().id);
      } else {
        sp->authorization_requested_ = false;
        try {
          std::rethrow_exception(result.error());
        } catch (const std::exception& e) {
          sp->log_.errorf(component, "failed to authenticate with AirMap services: %s", e.what());
        } catch (...) {
          sp->log_.errorf(component, "failed to authenticate with AirMap services");
        }
      }
    });
  }
}

void airmap::Daemon::TelemetrySubmitter::handle_request_authorization_finished(std::string authorization) {
  log_.infof(component, "successfully requested authorization from AirMap services %s", authorization);
  authorization_ = authorization;
  request_create_flight();
}

void airmap::Daemon::TelemetrySubmitter::request_create_flight() {
  if (flight_) {
    handle_request_create_flight_finished(flight_.get());
    return;
  }

  if (create_flight_requested_)
    return;

  create_flight_requested_ = true;

  if (current_position_) {
    Flights::CreateFlight::Parameters params;
    params.authorization = authorization_.get();
    params.latitude      = current_position_.get().lat / 1E7;
    params.longitude     = current_position_.get().lon / 1E7;
    params.aircraft_id   = aircraft_id_;
    params.start_time    = Clock::universal_time();
    params.end_time      = params.start_time + Hours{1};

    client_->flights().create_flight_by_point(params, [sp = shared_from_this()](const auto& result) {
      if (result) {
        sp->handle_request_create_flight_finished(result.value());
      } else {
        sp->create_flight_requested_ = false;
        try {
          std::rethrow_exception(result.error());
        } catch (const std::exception& e) {
          sp->log_.errorf(component, "failed to create flight by point: %s", e.what());
        } catch (...) {
          sp->log_.errorf(component, "failed to create flight by point");
        }
      }
    });
  }
}

void airmap::Daemon::TelemetrySubmitter::handle_request_create_flight_finished(Flight flight) {
  log_.infof(component, "successfully created flight: %s", flight.id);
  flight_ = flight;
  request_start_flight_comms();
}

void airmap::Daemon::TelemetrySubmitter::request_start_flight_comms() {
  if (encryption_key_) {
    handle_request_start_flight_comms_finished(encryption_key_.get());
    return;
  }

  if (start_flight_comms_requested_)
    return;

  start_flight_comms_requested_ = true;

  Flights::StartFlightCommunications::Parameters params{authorization_.get(), flight_.get().id};

  client_->flights().start_flight_communications(params, [sp = shared_from_this()](const auto& result) {
    if (result) {
      sp->handle_request_start_flight_comms_finished(result.value().key);
    } else {
      sp->start_flight_comms_requested_ = false;
      try {
        std::rethrow_exception(result.error());
      } catch (const std::exception& e) {
        sp->log_.errorf(component, "failed to start flight communications: %s", e.what());
      } catch (...) {
        sp->log_.errorf(component, "failed to start flight communications");
      }
    }
  });
}

void airmap::Daemon::TelemetrySubmitter::handle_request_start_flight_comms_finished(std::string key) {
  log_.infof(component, "successfully started flight comms: %s", key);
  encryption_key_ = key;
}

airmap::Daemon::SubmittingVehicleMonitor::SubmittingVehicleMonitor(const std::shared_ptr<TelemetrySubmitter>& submitter)
    : submitter_{submitter} {
}

void airmap::Daemon::SubmittingVehicleMonitor::on_system_status_changed(const Optional<mavlink::State>& old_state,
                                                                        mavlink::State new_state) {
  if (old_state) {
    switch (old_state.get()) {
      case MAV_STATE_UNINIT:
      case MAV_STATE_BOOT:
      case MAV_STATE_CALIBRATING:
      case MAV_STATE_STANDBY:
        if (new_state == MAV_STATE_ACTIVE) {
          submitter_->activate();
        }
        break;
      case MAV_STATE_ACTIVE:
      case MAV_STATE_CRITICAL:
      case MAV_STATE_EMERGENCY:
        switch (new_state) {
          case MAV_STATE_UNINIT:
          case MAV_STATE_BOOT:
          case MAV_STATE_CALIBRATING:
          case MAV_STATE_STANDBY:
            submitter_->deactivate();
            break;
        }
        break;
      default:
        break;
    }
  }
}

void airmap::Daemon::SubmittingVehicleMonitor::on_position_changed(
    const Optional<mavlink::GlobalPositionInt>& old_position, const mavlink::GlobalPositionInt& new_position) {
  submitter_->submit(new_position);
}
