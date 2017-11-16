#ifndef AIRMAP_MONITOR_TELEMETRY_SUBMITTER_H_
#define AIRMAP_MONITOR_TELEMETRY_SUBMITTER_H_

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/credentials.h>
#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>

#include <airmap/mavlink/global_position_int.h>
#include <airmap/util/formatting_logger.h>

#include <memory>
#include <string>

namespace airmap {
namespace monitor {

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

}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_MONITOR_TELEMETRY_SUBMITTER_H_
