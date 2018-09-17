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
#ifndef AIRMAP_MAVLINK_VEHICLE_TRACKER_H_
#define AIRMAP_MAVLINK_VEHICLE_TRACKER_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/mavlink/vehicle.h>
#include <airmap/util/formatting_logger.h>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif
#include <standard/mavlink.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace airmap {
namespace mavlink {

class VehicleTracker {
 public:
  class Monitor : DoNotCopyOrMove {
   public:
    virtual void on_vehicle_added(const std::shared_ptr<Vehicle>& vehicle)   = 0;
    virtual void on_vehicle_removed(const std::shared_ptr<Vehicle>& vehicle) = 0;

   protected:
    Monitor() = default;
  };

  void update(const mavlink_message_t& msg);

  void register_monitor(const std::shared_ptr<Monitor>& monitor);
  void unregister_monitor(const std::shared_ptr<Monitor>& monitor);

 protected:
  std::unordered_set<std::shared_ptr<Monitor>> monitors_;
  std::unordered_map<std::uint8_t, std::shared_ptr<Vehicle>> vehicles_;
};

class LoggingVehicleTrackerMonitor : public VehicleTracker::Monitor {
 public:
  explicit LoggingVehicleTrackerMonitor(const char* component, const std::shared_ptr<Logger>& logger,
                                        const std::shared_ptr<VehicleTracker::Monitor>& next);

  // From VehicleTracker::Monitor
  void on_vehicle_added(const std::shared_ptr<Vehicle>& vehicle) override;
  void on_vehicle_removed(const std::shared_ptr<Vehicle>& vehicle) override;

 private:
  const char* component_;
  util::FormattingLogger log_;
  std::shared_ptr<VehicleTracker::Monitor> next_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_VEHICLE_TRACKER_H_
