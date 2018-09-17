// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef AIRMAP_MAVLINK_VEHICLE_H_
#define AIRMAP_MAVLINK_VEHICLE_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>
#include <airmap/util/formatting_logger.h>

#include <airmap/mavlink/global_position_int.h>
#include <airmap/mavlink/mission.h>
#include <airmap/mavlink/state.h>

#include <cstdint>

#include <memory>
#include <unordered_set>

namespace airmap {
namespace mavlink {

class Vehicle {
 public:
  class Monitor : DoNotCopyOrMove {
   public:
    virtual void on_system_status_changed(const Optional<State>& old_state, State new_state) = 0;
    virtual void on_position_changed(const Optional<GlobalPositionInt>& old_position,
                                     const GlobalPositionInt& new_position)                  = 0;
    virtual void on_mission_received(const airmap::Geometry& geometry)                       = 0;

   protected:
    Monitor() = default;
  };
  using SystemId = std::uint8_t;

  explicit Vehicle(std::uint8_t system_id);

  void update(const mavlink_message_t& msg);
  void register_monitor(const std::shared_ptr<Monitor>& monitor);
  void unregister_monitor(const std::shared_ptr<Monitor>& monitor);

 protected:
  void handle_msg_global_position_int(const mavlink_message_t& msg);
  void handle_msg_heartbeat(const mavlink_message_t& msg);
  void handle_msg_mission(const mavlink_message_t& msg);

  std::uint8_t system_id_;
  std::unordered_set<std::shared_ptr<Monitor>> monitors_;

  State system_status_;
  Optional<GlobalPositionInt> global_position_int_;

  Mission mission_;
};

class LoggingVehicleMonitor : public Vehicle::Monitor {
 public:
  explicit LoggingVehicleMonitor(const char* component, const std::shared_ptr<Logger>& logger,
                                 const std::shared_ptr<Vehicle::Monitor>& next);

  // From Vehicle::Monitor
  void on_system_status_changed(const Optional<State>& old_state, State new_state) override;
  void on_position_changed(const Optional<GlobalPositionInt>& old_position,
                           const GlobalPositionInt& new_position) override;
  void on_mission_received(const airmap::Geometry& geometry) override;

 private:
  const char* component_;
  util::FormattingLogger log_;
  std::shared_ptr<Vehicle::Monitor> next_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_VEHICLE_H_
