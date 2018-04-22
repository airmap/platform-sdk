#include <airmap/monitor/submitting_vehicle_monitor.h>

airmap::monitor::SubmittingVehicleMonitor::SubmittingVehicleMonitor(
    const std::shared_ptr<TelemetrySubmitter>& submitter)
    : submitter_{submitter} {
}

void airmap::monitor::SubmittingVehicleMonitor::on_system_status_changed(const Optional<mavlink::State>& old_state,
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

void airmap::monitor::SubmittingVehicleMonitor::on_position_changed(
    const Optional<mavlink::GlobalPositionInt>& old_position, const mavlink::GlobalPositionInt& new_position) {
  submitter_->submit(new_position);
}

void airmap::monitor::SubmittingVehicleMonitor::on_mission_received(const airmap::Geometry& geometry) {
  submitter_->set_mission_geometry(geometry);
}
