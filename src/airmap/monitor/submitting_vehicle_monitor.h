//
//  submitting_vehicle_monitor.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MONITOR_SUBMITTING_VEHICLE_MONITOR_H_
#define AIRMAP_MONITOR_SUBMITTING_VEHICLE_MONITOR_H_

#include <airmap/mavlink/vehicle.h>
#include <airmap/monitor/telemetry_submitter.h>

#include <memory>

namespace airmap {
namespace monitor {

/// SubmittingVehicleMonitor monitors a mavlink::Vehicle.
///
/// SubmittingVehicleMonitor translates state changes and carries out
/// the following actions:
///   - vehicle becomes active
///     - create flight
///     - start flight comms
///   - new position estimate
///     - if vehicle active:
///       - submit telemetry
///   - vehicle becomes inactive
///     - end flight comms
///     - end flight
class SubmittingVehicleMonitor : public mavlink::Vehicle::Monitor {
 public:
  /// SubmittingVehicleMonitor initializes a new instance with 'submitter'.
  explicit SubmittingVehicleMonitor(const std::shared_ptr<TelemetrySubmitter>& submitter);

  // From Vehicle::Monitor
  void on_system_status_changed(const Optional<mavlink::State>& old_state, mavlink::State new_state) override;
  void on_position_changed(const Optional<mavlink::GlobalPositionInt>& old_position,
                           const mavlink::GlobalPositionInt& new_position) override;
  void on_mission_received(const airmap::Geometry& geometry) override;

 private:
  /// @cond
  std::shared_ptr<TelemetrySubmitter> submitter_;
  /// @endcond
};

}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_MONITOR_SUBMITTING_VEHICLE_MONITOR_H_
