#ifndef AIRMAP_MAVLINK_VEHICLE_TRACKER_H_
#define AIRMAP_MAVLINK_VEHICLE_TRACKER_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/mavlink/vehicle.h>
#include <airmap/util/formatting_logger.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#include <standard/mavlink.h>
#pragma clang diagnostic pop

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