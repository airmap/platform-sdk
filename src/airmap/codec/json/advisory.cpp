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
#include <airmap/codec/json/advisory.h>

#include <airmap/codec.h>
#include <airmap/codec/json/airspace.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>
#include <airmap/codec/json/status.h>

void airmap::codec::json::decode(const nlohmann::json& j, Advisory::AirspaceAdvisory& advisory) {
  get(advisory.color, j, "color");
  get(advisory.rule_id, j, "rule_id");
  get(advisory.ruleset_id, j, "ruleset_id");
  decode(j, advisory.advisory);
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Advisory::AirspaceAdvisory>& v) {
  for (auto element : j["advisories"]) {
    v.push_back(Advisory::AirspaceAdvisory{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Advisory::Weather& weather) {
  auto jw = j["weather"][0];
  if (!jw) { return; }
  get(weather.condition, jw, "condition");
  get(weather.icon, jw, "icon");
  get(weather.wind, jw, "wind");
  get(weather.humidity, jw, "humidity");
  get(weather.visibility, jw, "visibility");
  get(weather.precipitation, jw, "precipitation");
  get(weather.temperature, jw, "temperature");
  get(weather.timezone, jw, "timezone");
  get(weather.time, jw, "time");
  get(weather.dew_point, j, "dew_point");
  get(weather.mslp, jw, "mslp");
}

void airmap::codec::json::decode(const nlohmann::json& j, Advisory::Wind& wind) {
  get(wind.heading, j, "heading");
  get(wind.speed, j, "speed");
  get(wind.gusting, j, "gusting");
}
