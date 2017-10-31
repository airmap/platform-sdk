#include <airmap/cmds/airmap/cmd/simulate_scenario.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/paths.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/scenario_simulator.h>
#include <airmap/util/telemetry_simulator.h>

#include <boost/asio.hpp>

#include <signal.h>

#include <fstream>
#include <iterator>
#include <thread>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;
namespace ph  = std::placeholders;

using json = nlohmann::json;

namespace {
const std::string device_name = boost::asio::ip::host_name();
constexpr const char* component{"simulate-scenario-cli"};
}  // namespace

cmd::SimulateScenario::Collector::Collector(const util::Scenario& scenario) : scenario_{scenario} {
}

void cmd::SimulateScenario::Collector::collect_authentication_for(util::Scenario::Participants::iterator it,
                                                                  const std::string& authentication) {
  it->authentication = authentication;
}

void cmd::SimulateScenario::Collector::collect_authentication_for_index(std::size_t index,
                                                                        const std::string& authentication) {
  scenario_.participants.at(index).authentication = authentication;
}

void cmd::SimulateScenario::Collector::collect_flight_id_for(util::Scenario::Participants::iterator it,
                                                             const Flight& flight) {
  it->flight = flight;
}

void cmd::SimulateScenario::Collector::collect_flight_id_for_index(std::size_t index, const Flight& flight) {
  scenario_.participants.at(index).flight = flight;
}

void cmd::SimulateScenario::Collector::collect_traffic_monitor_for(util::Scenario::Participants::iterator it,
                                                                   const std::shared_ptr<Traffic::Monitor>& monitor) {
  it->monitor = monitor;
}

void cmd::SimulateScenario::Collector::collect_traffic_monitor_for_index(
    std::size_t index, const std::shared_ptr<Traffic::Monitor>& monitor) {
  scenario_.participants.at(index).monitor = monitor;
}

bool cmd::SimulateScenario::Collector::collect_key_for(util::Scenario::Participants::iterator it,
                                                       const std::string& key) {
  it->encryption_key = key;
  return scenario_.participants.size() == ++key_counter_;
}

bool cmd::SimulateScenario::Collector::collect_key_for_index(std::size_t index, const std::string& key) {
  scenario_.participants.at(index).encryption_key = key;
  return scenario_.participants.size() == ++key_counter_;
}

const airmap::util::Scenario& cmd::SimulateScenario::Collector::scenario() const {
  return scenario_;
}

airmap::util::Scenario& cmd::SimulateScenario::Collector::scenario() {
  return scenario_;
}

cmd::SimulateScenario::SimulateScenario()
    : cli::CommandWithFlagsAndAction{"simulate-scenario",
                                     "simulate multiple vehicles carrying out missions and submitting telemetry",
                                     "simulate multiple vehicles carrying out missions and submitting telemetry"} {
  flag(flags::version(params_.version));
  flag(flags::log_level(params_.log_level));
  flag(flags::config_file(params_.config_file));
  flag(flags::telemetry_host(params_.host));
  flag(flags::telemetry_port(params_.port));
  flag(cli::make_flag("scenario-file", "use the scenario defined in this json file", params_.scenario_file));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(params_.log_level, create_default_logger(ctxt.cout))};

    if (!params_.config_file) {
      params_.config_file = ConfigFile{paths::config_file(params_.version).string()};
    }

    if (params_.host && !params_.host.get().validate()) {
      log_.errorf(component, "parameter 'host' must not be empty");
      return 1;
    }

    if (!params_.scenario_file) {
      log_.errorf(component, "missing parameter 'scenario-file'");
      return 1;
    }

    std::ifstream in{params_.scenario_file.get()};
    if (!in) {
      log_.errorf(component, "failed to open %s for reading", params_.scenario_file.get());
      return 1;
    }
    util::Scenario scenario = json::parse(in);

    collector_  = std::make_shared<Collector>(scenario);
    runner_     = std::make_shared<util::ScenarioSimulator::Runner>(5);
    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "could not acquire resources for accessing AirMap services");
      return 1;
    }

    context_ = result.value();

    std::ifstream config_file{params_.config_file.get()};
    if (!config_file) {
      log_.errorf(component, "failed to open config file %s for reading", params_.config_file.get());
      return 1;
    }
    auto config = Client::load_configuration_from_json(config_file);

    if (params_.host)
      config.telemetry.host = params_.host.get();
    if (params_.port)
      config.telemetry.port = params_.port.get();

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(config, [this](const auto& result) mutable {
      if (not result) {
        try {
          std::rethrow_exception(result.error());
        } catch (const std::exception& e) {
          log_.errorf(component, "failed to create client: %s", e.what());
        } catch (...) {
          log_.errorf(component, "failed to create client");
        }
        context_->stop(::airmap::Context::ReturnCode::error);
        return;
      }
      log_.infof(component, "successfully created client for AirMap services");

      client_  = result.value();
      auto it  = collector_->scenario().participants.begin();
      auto itE = collector_->scenario().participants.end();

      while (it != itE) {
        this->request_authentication_for(it);
        ++it;
      }
    });

    return context_->exec({SIGINT, SIGQUIT},
                          [this](int sig) {
                            log_.infof(component, "received [%s], shutting down", ::strsignal(sig));
                            context_->stop();
                          }) == ::airmap::Context::ReturnCode::success
               ? 0
               : 1;
  });
}

void cmd::SimulateScenario::request_authentication_for(util::Scenario::Participants::iterator participant) {
  if (participant->user) {
    Authenticator::AuthenticateWithPassword::Params params;
    params.oauth.client_id = participant->user.get().client_id;
    params.oauth.username  = participant->user.get().username;
    params.oauth.password  = participant->user.get().password;
    params.oauth.device_id = device_name;
    client_->authenticator().authenticate_with_password(
        params, std::bind(&SimulateScenario::handle_authenticated_with_password_result_for, this, participant, ph::_1));
  } else {
    client_->authenticator().authenticate_anonymously(
        {participant->pilot.id},
        std::bind(&SimulateScenario::handle_authenticated_anonymously_result_for, this, participant, ph::_1));
  }
}

void cmd::SimulateScenario::handle_authenticated_with_password_result_for(
    util::Scenario::Participants::iterator participant, const Authenticator::AuthenticateWithPassword::Result& result) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    collector_->collect_authentication_for(participant, result.value().id);
    request_flight_status_for(participant);
    request_create_flight_for(participant);
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not authenticate with the Airmap services: %s", e.what());
    } catch (...) {
      log_.errorf(component, "could not authenticate with the Airmap services");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
void cmd::SimulateScenario::handle_authenticated_anonymously_result_for(
    util::Scenario::Participants::iterator participant, const Authenticator::AuthenticateAnonymously::Result& result) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    collector_->collect_authentication_for(participant, result.value().id);
    request_flight_status_for(participant);
    request_create_flight_for(participant);
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not authenticate with the Airmap services: %s", e.what());
    } catch (...) {
      log_.errorf(component, "could not authenticate with the Airmap services");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::SimulateScenario::request_flight_status_for(util::Scenario::Participants::iterator participant) {
  Status::GetStatus::Parameters params;
  const auto& polygon     = participant->geometry.details_for_polygon();
  params.latitude         = polygon[0].coordinates[0].latitude;
  params.longitude        = polygon[0].coordinates[0].longitude;
  params.geometry         = participant->geometry;
  params.flight_date_time = Clock::universal_time();
  params.weather          = true;

  client_->status().get_status_by_polygon(
      params, std::bind(&SimulateScenario::handle_flight_status_result_for, this, participant, ph::_1));
}

void cmd::SimulateScenario::handle_flight_status_result_for(util::Scenario::Participants::iterator,
                                                            const Status::GetStatus::Result& result) {
  if (result) {
    log_.infof(component,
               "successfully received status:\n"
               "  max-safe-distance: %d\n"
               "  advisory-color:    %s\n"
               "  weather:             \n"
               "    condition:       %s\n"
               "    temperature:     %s\n"
               "    humidity:        %s\n"
               "    visibility:      %d\n"
               "    precipitation:   %d\n"
               "    wind:\n"
               "      heading:       %d\n"
               "      speed:         %d\n"
               "      gusting:       %d\n",
               result.value().max_safe_distance, result.value().advisory_color, result.value().weather.condition,
               result.value().weather.temperature, result.value().weather.humidity, result.value().weather.visibility,
               result.value().weather.precipitation, result.value().weather.wind.heading,
               result.value().weather.wind.speed, result.value().weather.wind.gusting);
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to get flight status: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to get flight status");
    }
  }
}

void cmd::SimulateScenario::request_create_flight_for(util::Scenario::Participants::iterator participant) {
  Flights::CreateFlight::Parameters params;
  const auto& polygon  = participant->geometry.details_for_polygon();
  params.authorization = participant->authentication.get();
  params.start_time    = Clock::universal_time();
  params.end_time      = Clock::universal_time() + Minutes{1};
  params.aircraft_id   = participant->aircraft.id;
  params.latitude      = polygon[0].coordinates[0].latitude;
  params.longitude     = polygon[0].coordinates[0].longitude;
  params.geometry      = participant->geometry;

  client_->flights().create_flight_by_polygon(
      params, std::bind(&SimulateScenario::handle_create_flight_result_for, this, participant, ph::_1));
}

void cmd::SimulateScenario::handle_create_flight_result_for(util::Scenario::Participants::iterator participant,
                                                            const Flights::CreateFlight::Result& result) {
  if (result) {
    log_.infof(component, "successfully created flight for polygon");
    collector_->collect_flight_id_for(participant, result.value());
    request_traffic_monitoring_for(participant);
    request_start_flight_comms_for(participant);
    static const Microseconds timeout{60 * 1000 * 1000};

    context_->schedule_in(timeout, [this, participant]() {
      client_->flights().end_flight_communications(
          {participant->authentication.get(), participant->flight.get().id},
          std::bind(&SimulateScenario::handle_end_flight_comms, this, participant, ph::_1));
    });

  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not create flight for polygon: %s", e.what());
    } catch (...) {
      log_.errorf(component, "could not create flight for polygon");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::SimulateScenario::request_traffic_monitoring_for(util::Scenario::Participants::iterator participant) {
  client_->traffic().monitor(
      {participant->flight.get().id, participant->authentication.get()},
      std::bind(&SimulateScenario::handle_traffic_monitoring_result_for, this, participant, ph::_1));
}

void cmd::SimulateScenario::handle_traffic_monitoring_result_for(util::Scenario::Participants::iterator participant,
                                                                 const Traffic::Monitor::Result& result) {
  if (result) {
    log_.infof(component, "successfully started monitoring traffic for flight: %s", participant->flight.get().id);
    auto monitor = result.value();
    monitor->subscribe(std::make_shared<Traffic::Monitor::LoggingSubscriber>(component, log_.logger()));
    collector_->collect_traffic_monitor_for(participant, monitor);
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not start monitoring traffic for flight %s: %s", participant->flight.get().id,
                  e.what());
    } catch (...) {
      log_.errorf(component, "could not start monitoring traffic for flight %s", participant->flight.get().id);
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::SimulateScenario::request_start_flight_comms_for(util::Scenario::Participants::iterator participant) {
  client_->flights().start_flight_communications(
      {participant->authentication.get(), participant->flight.get().id},
      std::bind(&SimulateScenario::handle_start_flight_comms_result_for, this, participant, ph::_1));
}

void cmd::SimulateScenario::handle_start_flight_comms_result_for(
    util::Scenario::Participants::iterator participant, const Flights::StartFlightCommunications::Result& result) {
  if (result) {
    log_.infof(component, "successfully started flight comms");

    if (collector_->collect_key_for(participant, result.value().key)) {
      auto simulator = std::make_shared<util::ScenarioSimulator>(collector_->scenario(), log_.logger());
      runner_->start(simulator, client_);
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not start flight comms for flight %s: %s", participant->flight.get().id, e.what());
    } catch (...) {
      log_.errorf(component, "could not start flight comms for flight %s", participant->flight.get().id);
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::SimulateScenario::handle_end_flight_comms(util::Scenario::Participants::iterator participant,
                                                    const Flights::EndFlightCommunications::Result& result) {
  if (result) {
    log_.infof(component, "successfully ended flight comms");
    client_->flights().end_flight({participant->authentication.get(), participant->flight.get().id},
                                  std::bind(&SimulateScenario::handle_end_flight, this, participant, ph::_1));
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to end flight comms: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to end flight comms");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::SimulateScenario::handle_end_flight(util::Scenario::Participants::iterator participant,
                                              const Flights::EndFlight::Result& result) {
  if (result) {
    log_.infof(component, "successfully finished flight: %s", participant->flight.get().id);
    context_->stop();
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to end flight: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to end flight");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
