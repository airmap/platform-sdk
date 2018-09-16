/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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
