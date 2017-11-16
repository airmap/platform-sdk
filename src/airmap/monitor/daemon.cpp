#include <airmap/monitor/daemon.h>

namespace {
constexpr const char* component{"airmap::monitor::Daemon"};
}  // namespace

std::shared_ptr<airmap::monitor::Daemon> airmap::monitor::Daemon::create(const Configuration& configuration) {
  return std::shared_ptr<Daemon> {
    new Daemon {
      configuration
    }
  }
  ->finalize();
}

airmap::monitor::Daemon::Daemon(const Configuration& configuration)
    : configuration_{configuration}, log_{configuration_.logger} {
}

std::shared_ptr<airmap::monitor::Daemon> airmap::monitor::Daemon::finalize() {
  auto sp                  = shared_from_this();
  vehicle_tracker_monitor_ = std::make_shared<mavlink::LoggingVehicleTrackerMonitor>(component, log_.logger(), sp);

  vehicle_tracker_.register_monitor(vehicle_tracker_monitor_);

  return sp;
}

airmap::monitor::Daemon::~Daemon() {
  configuration_.channel->stop();
  configuration_.channel->unsubscribe(std::move(mavlink_channel_subscription_));
  vehicle_tracker_.unregister_monitor(shared_from_this());
}

void airmap::monitor::Daemon::start() {
  mavlink_channel_subscription_ = configuration_.channel->subscribe([sp = shared_from_this()](
      const mavlink_message_t& msg) { sp->handle_mavlink_message(msg); });
  configuration_.channel->start();
}

void airmap::monitor::Daemon::handle_mavlink_message(const mavlink_message_t& msg) {
  vehicle_tracker_.update(msg);
}

void airmap::monitor::Daemon::on_vehicle_added(const std::shared_ptr<mavlink::Vehicle>& vehicle) {
  auto submitter = TelemetrySubmitter::create(configuration_.credentials, configuration_.aircraft_id, log_.logger(),
                                              configuration_.client);
  vehicle->register_monitor(std::make_shared<mavlink::LoggingVehicleMonitor>(
      component, log_.logger(), std::make_shared<SubmittingVehicleMonitor>(submitter)));
}

void airmap::monitor::Daemon::on_vehicle_removed(const std::shared_ptr<mavlink::Vehicle>&) {
  // empty on purpose
}

airmap::monitor::Daemon::SubmittingVehicleMonitor::SubmittingVehicleMonitor(
    const std::shared_ptr<TelemetrySubmitter>& submitter)
    : submitter_{submitter} {
}

void airmap::monitor::Daemon::SubmittingVehicleMonitor::on_system_status_changed(
    const Optional<mavlink::State>& old_state, mavlink::State new_state) {
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

void airmap::monitor::Daemon::SubmittingVehicleMonitor::on_position_changed(
    const Optional<mavlink::GlobalPositionInt>& old_position, const mavlink::GlobalPositionInt& new_position) {
  submitter_->submit(new_position);
}
