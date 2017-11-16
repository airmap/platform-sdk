#ifndef AIRMAP_DAEMON_H_
#define AIRMAP_DAEMON_H_

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/mavlink/channel.h>
#include <airmap/mavlink/vehicle.h>
#include <airmap/mavlink/vehicle_tracker.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>
#include <airmap/util/formatting_logger.h>

#include <memory>

namespace airmap {

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
  // TelemetrySubmitter implements a state machine that handles:
  //  - authorization
  //  - flight creation
  //  - flight comms startup
  //  - telemetry submission
  // whenever it becomes active and as soon as a current position of the
  // vehicle is known.
  //
  // TODO(tvoss): Replace the custom state machine implementation presented here with
  // a formal model expresses via boost::msm.
  class TelemetrySubmitter : public std::enable_shared_from_this<TelemetrySubmitter> {
   public:
    enum class State { active, inactive };

    static std::shared_ptr<TelemetrySubmitter> create(const Credentials& credentials, const std::string& aircraft_id,
                                                      const std::shared_ptr<Logger>& logger,
                                                      const std::shared_ptr<Client>& client);

    void activate();
    void deactivate();

    void submit(const mavlink::GlobalPositionInt&);

   private:
    explicit TelemetrySubmitter(const Credentials& credentials, const std::string& aircraft_id,
                                const std::shared_ptr<Logger>& logger, const std::shared_ptr<Client>& client);

    void request_authorization();
    void handle_request_authorization_finished(std::string authorization);

    void request_create_flight();
    void handle_request_create_flight_finished(Flight flight);

    void request_start_flight_comms();
    void handle_request_start_flight_comms_finished(std::string key);

    State state_{State::inactive};
    bool authorization_requested_{false};
    bool create_flight_requested_{false};
    bool start_flight_comms_requested_{false};

    util::FormattingLogger log_;
    std::shared_ptr<Client> client_;
    Credentials credentials_;
    std::string aircraft_id_;

    Optional<mavlink::GlobalPositionInt> current_position_;
    Optional<Flight> flight_;
    Optional<std::string> authorization_;
    Optional<std::string> encryption_key_;
  };

  class SubmittingVehicleMonitor : public mavlink::Vehicle::Monitor {
   public:
    explicit SubmittingVehicleMonitor(const std::shared_ptr<TelemetrySubmitter>& submitter);

    // From Vehicle::Monitor
    void on_system_status_changed(const Optional<mavlink::State>& old_state, mavlink::State new_state) override;
    void on_position_changed(const Optional<mavlink::GlobalPositionInt>& old_position,
                             const mavlink::GlobalPositionInt& new_position) override;

   private:
    std::shared_ptr<TelemetrySubmitter> submitter_;
  };

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

}  // namespace airmap

#endif  // AIRMAP_DAEMON_H_
