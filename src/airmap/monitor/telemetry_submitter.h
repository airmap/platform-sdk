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
#ifndef AIRMAP_MONITOR_TELEMETRY_SUBMITTER_H_
#define AIRMAP_MONITOR_TELEMETRY_SUBMITTER_H_

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/credentials.h>
#include <airmap/flight.h>
#include <airmap/flight_plan.h>
#include <airmap/flight_plans.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/pilot.h>
#include <airmap/pilots.h>
#include <airmap/telemetry.h>
#include <airmap/traffic.h>

#include <airmap/mavlink/global_position_int.h>
#include <airmap/util/formatting_logger.h>

#include <memory>
#include <string>

namespace airmap {
namespace monitor {

/// TelemetrySubmitter implements a state machine that handles:
///  - authorization
///  - flight creation
///  - flight comms startup
///  - telemetry submission
/// whenever it becomes active and as soon as a current position of the
/// vehicle is known.
///
/// TODO(tvoss): Replace the custom state machine implementation presented here with
/// a formal model expresses via boost::msm.
class TelemetrySubmitter : public std::enable_shared_from_this<TelemetrySubmitter> {
 public:
  /// State models all known states of the state machine.
  enum class State {
    active,   ///< active and processing telemetry submissions
    inactive  ///< inactive, dropping all telemetry submissions
  };

  /// create returns a new TelemetrySubmitter instance.
  static std::shared_ptr<TelemetrySubmitter> create(
      const Credentials& credentials, const std::string& aircraft_id, const std::shared_ptr<Logger>& logger,
      const std::shared_ptr<airmap::Client>& client,
      const std::shared_ptr<Traffic::Monitor::Subscriber>& traffic_subscriber);
  /// activate transitions an instance to State::active.
  ///
  /// The following sequence of actions is triggered:
  ///   * request authorization
  ///   * request flight creation
  ///   * request to start flight communications
  void activate();

  /// deactivate transitions an instance to State::inactive.
  ///
  /// The following sequence of actions is triggered:
  ///   * request to end flight communications
  ///   * request to end the flight
  void deactivate();

  /// submit requests an instance to submit a telemetry update.
  void submit(const mavlink::GlobalPositionInt&);

  /// set_mission_geometry announces the mission geometry.
  void set_mission_geometry(const Geometry& geometry);

 private:
  explicit TelemetrySubmitter(const Credentials& credentials, const std::string& aircraft_id,
                              const std::shared_ptr<Logger>& logger, const std::shared_ptr<airmap::Client>& client,
                              const std::shared_ptr<Traffic::Monitor::Subscriber>& traffic_subscriber);

  void request_authorization();
  void handle_request_authorization_finished(std::string authorization);

  void request_pilot_id();
  void handle_request_pilot_id_finished(std::string pilot_id);

  void request_active_flights();
  void handle_request_active_flights_finished(std::vector<Flight> flight);

  void request_end_active_flights();
  void handle_request_end_active_flight_finished(std::string id);
  void handle_request_end_active_flights_finished();

  void request_create_flight_plan();
  void handle_request_create_flight_plan_finished(FlightPlan plan);

  void request_submit_flight_plan();
  void handle_request_submit_flight_plan_finished(Flight flight);

  void request_monitor_traffic();
  void handle_request_monitor_traffic_finished(std::shared_ptr<Traffic::Monitor> traffic_monitor);

  void request_start_flight_comms();
  void handle_request_start_flight_comms_finished(std::string key);

  State state_{State::inactive};
  bool authorization_requested_{false};
  bool pilot_id_requested_{false};
  bool active_flights_requested_{false};
  bool end_active_flights_requested_{false};
  bool create_flight_plan_requested_{false};
  bool submit_flight_plan_requested_{false};
  bool traffic_monitoring_requested_{false};
  bool start_flight_comms_requested_{false};

  util::FormattingLogger log_;
  std::shared_ptr<airmap::Client> client_;
  std::shared_ptr<Traffic::Monitor::Subscriber> traffic_subscriber_;
  Credentials credentials_;
  std::string aircraft_id_;

  Optional<mavlink::GlobalPositionInt> current_position_;
  Optional<std::vector<Flight>> active_flights_;
  Optional<FlightPlan> flight_plan_;
  Optional<Flight> flight_;
  Optional<std::shared_ptr<Traffic::Monitor>> traffic_monitor_;
  Optional<std::string> encryption_key_;
  Optional<Geometry> mission_geometry_;
  Optional<std::string> pilot_id_;
};

}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_MONITOR_TELEMETRY_SUBMITTER_H_
