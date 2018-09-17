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
#include <airmap/codec/json/flight_plan.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlans::Delete::Response& r) {
  get(r.id, j, "id");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlans::Create::Parameters& p) {
  get(p.latitude, j, "takeoff_latitude");
  get(p.longitude, j, "takeoff_longitude");
  get(p.max_altitude, j, "max_altitude_agl");
  get(p.min_altitude, j, "min_altitude_agl");
  get(p.geometry, j, "geometry");
  get(p.start_time, j, "start_time");
  get(p.end_time, j, "end_time");
  get(p.pilot.id, j, "pilot_id");
  if (j.count("aircraft_id") > 0) {
    Pilot::Aircraft aircraft;
    aircraft.id = j["aircraft_id"];
    p.aircraft.set(aircraft);
  }
  get(p.buffer, j, "buffer");
  get(p.rulesets, j, "rulesets");
  if (j.count("flight_features") > 0) {
    auto ff = j["flight_features"];

    for (auto it = ff.begin(); it != ff.end(); ++it) {
      RuleSet::Feature::Value value = it.value();
      p.features.emplace(it.key(), value);
    }
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const FlightPlans::Create::Parameters& p) {
  j["takeoff_latitude"]  = p.latitude;
  j["takeoff_longitude"] = p.longitude;
  j["max_altitude_agl"]  = p.max_altitude;
  j["min_altitude_agl"]  = p.min_altitude;
  j["geometry"]          = p.geometry;
  j["start_time"]        = p.start_time;
  j["end_time"]          = p.end_time;
  j["pilot_id"]          = p.pilot.id;
  if (p.aircraft)
    j["aircraft_id"] = p.aircraft.get().id;
  j["buffer"] = p.buffer;

  for (const auto& id : p.rulesets)
    j["rulesets"].push_back(id);

  for (const auto& pair : p.features) {
    j["flight_features"][pair.first] = pair.second;
  }
}
