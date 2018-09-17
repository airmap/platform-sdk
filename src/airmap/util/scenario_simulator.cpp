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
#include <airmap/util/scenario_simulator.h>

#include <airmap/codec.h>
#include <airmap/telemetry.h>

#include <chrono>

namespace {
constexpr const char* component{"ScenarioSimulator"};
}  // namespace

airmap::util::ScenarioSimulator::Runner::Runner(std::uint32_t frequency) : frequency_{frequency}, running_{false} {
}

airmap::util::ScenarioSimulator::Runner::~Runner() {
  stop();
  if (worker_.joinable())
    worker_.join();
}

void airmap::util::ScenarioSimulator::Runner::start(
    const std::shared_ptr<ScenarioSimulator>& simulator,
    const std::function<void(const DateTime&, const Scenario::Participant&, const Geometry::Coordinate&)>& receiver) {
  if (!running_.exchange(true)) {
    worker_ = std::thread{[this, simulator, receiver]() {
      simulator->start();
      while (running_.load()) {
        simulator->update(receiver);
        std::this_thread::sleep_for(std::chrono::milliseconds{1000 / frequency_});
      }
    }};
  }
}

void airmap::util::ScenarioSimulator::Runner::stop() {
  running_.exchange(false);
}

airmap::util::ScenarioSimulator::ScenarioSimulator(const Scenario& scenario, const std::shared_ptr<Logger>& logger)
    : scenario_{scenario}, log_{logger} {
  for (const auto& participant : scenario_.participants) {
    state_.emplace_back(participant.geometry.details_for_polygon());
  }
}

void airmap::util::ScenarioSimulator::start() {
  last_update_ = Clock::universal_time();
}

void airmap::util::ScenarioSimulator::update(
    const std::function<void(const DateTime&, const Scenario::Participant&, const Geometry::Coordinate&)>& enumerator) {
  auto now = Clock::universal_time();

  for (std::size_t i = 0; i < scenario_.participants.size(); i++) {
    const auto& participant = scenario_.participants.at(i);
    auto new_state          = state_.at(i).update(now);
    log_.debugf(component, "updating participant %s on %s: (%f, %f)", participant.pilot.user_name,
                participant.aircraft.id, new_state.latitude, new_state.longitude);

    enumerator(now, participant, new_state);
  }

  last_update_ = now;
}

void airmap::util::from_json(const nlohmann::json& j, Scenario& s) {
  codec::json::get(s.name, j, "name");
  codec::json::get(s.description, j, "description");
  codec::json::get(s.participants, j, "participants");
  codec::json::get(s.duration, j, "duration");
}

void airmap::util::from_json(const nlohmann::json& j, Scenario::Participant& p) {
  codec::json::get(p.id, j, "id");
  codec::json::get(p.aircraft, j, "aircraft");
  codec::json::get(p.pilot, j, "pilot");
  codec::json::get(p.user, j, "user");
  codec::json::get(p.geometry, j, "geometry");
  codec::json::get(p.velocity, j, "velocity");
}

void airmap::util::from_json(const nlohmann::json& j, Scenario::User& u) {
  codec::json::get(u.username, j, "username");
  codec::json::get(u.password, j, "password");
  codec::json::get(u.client_id, j, "client_id");
}

void airmap::util::from_json(const nlohmann::json& j, std::vector<Scenario::Participant>& p) {
  for (const auto& element : j) {
    p.push_back(Scenario::Participant{});
    p.back() = element;
  }
}
