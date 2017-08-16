#ifndef AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_SCENARIO_H_
#define AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_SCENARIO_H_

#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>
#include <airmap/traffic.h>
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
  using ApiKey       = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Host         = util::TaggedString<util::tags::MustNotBeEmpty>;
  using ScenarioFile = util::TaggedString<util::tags::MustNotBeEmpty>;

  class Collector {
   public:
    explicit Collector(const util::Scenario& scenario);
    void collect_authentication_for_index(std::size_t index, const std::string& authentication);
    void collect_flight_id_for_index(std::size_t index, const Flight& flight);
    void collect_traffic_monitor_for_index(std::size_t index, const std::shared_ptr<Traffic::Monitor>& monitor);
    bool collect_key_for_index(std::size_t index, const std::string& key);
    const util::Scenario& scenario() const;

   private:
    uint key_counter_{0};
    util::Scenario scenario_;
  };

  struct {
    Client::Version version{Client::Version::production};
    Optional<ApiKey> api_key;
    Optional<Host> host;
    Optional<std::uint16_t> port;
    Optional<ScenarioFile> scenario_file;
  } params_;
  std::shared_ptr<util::ScenarioSimulator::Runner> runner_;
  util::FormattingLogger log_{create_null_logger()};
  std::shared_ptr<Collector> collector_;
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<Client> client_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_SCENARIO_H_