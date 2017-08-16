#ifndef AIRMAP_UTIL_SCENARIO_SIMULATOR_H_
#define AIRMAP_UTIL_SCENARIO_SIMULATOR_H_

#include <airmap/aircraft.h>
#include <airmap/client.h>
#include <airmap/date_time.h>
#include <airmap/flight.h>
#include <airmap/geometry.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/pilot.h>
#include <airmap/traffic.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/telemetry_simulator.h>

#include <nlohmann/json.hpp>

#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <vector>

namespace airmap {
namespace util {

struct Scenario {
  struct Participant {
    Pilot::Aircraft aircraft;
    Pilot pilot;
    Geometry geometry;
    double velocity;

    Optional<std::string> authentication;
    Optional<Flight> flight;
    Optional<std::shared_ptr<Traffic::Monitor>> monitor;
    Optional<std::string> encryption_key;
  };

  std::string name;
  std::string description;
  std::vector<Participant> participants;
};

class ScenarioSimulator {
 public:
  class Runner {
   public:
    explicit Runner(std::uint32_t frequency);
    ~Runner();

    void start(const std::shared_ptr<ScenarioSimulator>& simulator, const std::shared_ptr<Client>& client);
    void stop();

   private:
    std::uint32_t frequency_;
    std::thread worker_;
    std::atomic<bool> running_;
  };

  explicit ScenarioSimulator(const Scenario& scenario, const std::shared_ptr<Logger>& logger);

  /// Start sets up the state of the simulator and prepares
  /// for subsequent calls to update.
  void start();

  /// update advances the simulation, and submits telemetry
  /// updates for all participants in the simulation.
  void update(const std::function<void(const DateTime&, const Scenario::Participant&, const Geometry::Coordinate&)>&
                  enumerator);

 private:
  Scenario scenario_;
  std::vector<TelemetrySimulator> state_;
  util::FormattingLogger log_;
  DateTime last_update_;
};

void from_json(const nlohmann::json& j, Scenario& scenario);
void from_json(const nlohmann::json& j, Scenario::Participant& participant);
void from_json(const nlohmann::json& j, std::vector<Scenario::Participant>& p);

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_SCENARIO_SIMULATOR_H_