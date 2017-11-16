#ifndef AIRMAP_MONITOR_DAEMON_H_
#define AIRMAP_MONITOR_DAEMON_H_

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/mavlink/channel.h>
#include <airmap/mavlink/vehicle.h>
#include <airmap/mavlink/vehicle_tracker.h>
#include <airmap/monitor/telemetry_submitter.h>
#include <airmap/util/formatting_logger.h>

#include <memory>

namespace airmap {
namespace monitor {

/// Daemon respresents AirMap and all of its services within
/// the system. It listens to incoming data (both in terms of
/// sensors as well as in terms of control messages) and enables
/// a drone operation platform to deeply integrate airspace intelligence
/// in its operations.
class Daemon : public mavlink::VehicleTracker::Monitor, public std::enable_shared_from_this<Daemon> {
 public:
  struct Configuration {
    Credentials credentials;
    std::string aircraft_id;
    std::shared_ptr<Logger> logger;
    std::shared_ptr<mavlink::Channel> channel;
    std::shared_ptr<Client> client;
  };

  // create returns a new Daemon instance ready for startup.
  static std::shared_ptr<Daemon> create(const Configuration& configuration);

  ~Daemon();

  /// Puts the daemon to execution.
  void start();

 protected:
  // From VehicleTracker::Monitor
  void on_vehicle_added(const std::shared_ptr<mavlink::Vehicle>& vehicle) override;
  void on_vehicle_removed(const std::shared_ptr<mavlink::Vehicle>& vehicle) override;

 private:
  /// Daemon initializes a new instance with the functional
  /// dependencies provided via 'configuration'.
  explicit Daemon(const Configuration& configuration);

  /// finalize sets up all internal event connections that require
  /// shared_from_this() to work properly.
  std::shared_ptr<Daemon> finalize();

  void handle_mavlink_message(const mavlink_message_t& msg);

  Configuration configuration_;

  util::FormattingLogger log_;
  std::shared_ptr<mavlink::LoggingVehicleTrackerMonitor> vehicle_tracker_monitor_;
  mavlink::VehicleTracker vehicle_tracker_;
  mavlink::Channel::Subscription mavlink_channel_subscription_;
};

}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_DAEMON_H_
