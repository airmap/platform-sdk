#ifndef AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_SCENARIO_H_
#define AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_SCENARIO_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/status.h>
#include <airmap/telemetry.h>
#include <airmap/traffic.h>

#include <airmap/boost/context.h>
#include <airmap/mavlink/router.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/scenario_simulator.h>
#include <airmap/util/tagged_string.h>

#include <memory>
#include <string>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class SimulateScenario : public util::cli::CommandWithFlagsAndAction {
 public:
  SimulateScenario();

 private:
  using ScenarioFile = util::TaggedString<util::tags::MustNotBeEmpty>;

  class Collector {
   public:
    explicit Collector(const util::Scenario& scenario);
    void collect_authentication_for(util::Scenario::Participants::iterator it, const std::string& authentication);
    void collect_authentication_for_index(std::size_t index, const std::string& authentication);
    void collect_flight_id_for(util::Scenario::Participants::iterator it, const Flight& flight);
    void collect_flight_id_for_index(std::size_t index, const Flight& flight);
    void collect_traffic_monitor_for(util::Scenario::Participants::iterator it,
                                     const std::shared_ptr<Traffic::Monitor>& monitor);
    void collect_traffic_monitor_for_index(std::size_t index, const std::shared_ptr<Traffic::Monitor>& monitor);
    bool collect_key_for(util::Scenario::Participants::iterator it, const std::string& key);
    bool collect_key_for_index(std::size_t index, const std::string& key);

    const util::Scenario& scenario() const;
    util::Scenario& scenario();

   private:
    unsigned int key_counter_{0};
    util::Scenario scenario_;
  };

  void request_authentication_for(util::Scenario::Participants::iterator participant);
  void handle_authenticated_with_password_result_for(util::Scenario::Participants::iterator participant,
                                                     const Authenticator::AuthenticateWithPassword::Result& result);
  void handle_authenticated_anonymously_result_for(util::Scenario::Participants::iterator participant,
                                                   const Authenticator::AuthenticateAnonymously::Result& result);

  void request_flight_status_for(util::Scenario::Participants::iterator participant);
  void handle_flight_status_result_for(util::Scenario::Participants::iterator participant,
                                       const Status::GetStatus::Result& result);

  void request_create_flight_for(util::Scenario::Participants::iterator participant);
  void handle_create_flight_result_for(util::Scenario::Participants::iterator participant,
                                       const Flights::CreateFlight::Result& result);

  void request_traffic_monitoring_for(util::Scenario::Participants::iterator participant);
  void handle_traffic_monitoring_result_for(util::Scenario::Participants::iterator participant,
                                            const Traffic::Monitor::Result& result);

  void request_start_flight_comms_for(util::Scenario::Participants::iterator participant);
  void handle_start_flight_comms_result_for(util::Scenario::Participants::iterator participant,
                                            const Flights::StartFlightCommunications::Result& result);

  void handle_end_flight_comms(util::Scenario::Participants::iterator participant,
                               const Flights::EndFlightCommunications::Result& result);
  void handle_end_flight(util::Scenario::Participants::iterator participant, const Flights::EndFlight::Result& result);

  struct {
    Client::Version version{Client::Version::production};
    Logger::Severity log_level{Logger::Severity::info};
    Optional<ConfigFile> config_file;
    Required<TelemetryHost> host;
    Required<std::uint16_t> port;
    Required<std::uint16_t> mavlink_router_endpoint_port;
    Required<ScenarioFile> scenario_file;
  } params_;
  std::shared_ptr<util::ScenarioSimulator::Runner> runner_;
  util::FormattingLogger log_{create_null_logger()};
  std::shared_ptr<Collector> collector_;
  std::shared_ptr<::airmap::boost::Context> context_;
  std::shared_ptr<::airmap::mavlink::Router> router_;
  std::shared_ptr<Client> client_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_SCENARIO_H_
