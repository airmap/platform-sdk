/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  scenario_simulator.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_UTIL_SCENARIO_SIMULATOR_H_
#define AIRMAP_UTIL_SCENARIO_SIMULATOR_H_

#include <airmap/aircraft.h>
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
  struct User {
    std::string username;
    std::string password;
    std::string client_id;
  };

  struct Participant {
    std::uint32_t id;
    Pilot::Aircraft aircraft;
    Pilot pilot;
    Optional<User> user;
    Geometry geometry;
    double velocity;

    Optional<std::string> authentication;
    Optional<Flight> flight;
    Optional<std::shared_ptr<Traffic::Monitor>> monitor;
    Optional<std::string> encryption_key;
  };

  using Participants = std::vector<Participant>;

  std::string name;
  std::string description;
  Participants participants;
  Optional<Seconds> duration;
};

class ScenarioSimulator {
 public:
  class Runner {
   public:
    explicit Runner(std::uint32_t frequency);
    ~Runner();

    void start(const std::shared_ptr<ScenarioSimulator>& simulator,
               const std::function<void(const DateTime&, const Scenario::Participant&, const Geometry::Coordinate&)>&
                   enumerator);
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
void from_json(const nlohmann::json& j, Scenario::User& user);
void from_json(const nlohmann::json& j, std::vector<Scenario::Participant>& p);

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_SCENARIO_SIMULATOR_H_
