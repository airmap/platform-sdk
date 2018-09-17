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
#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/rulesets.h>

void airmap::codec::json::decode(const nlohmann::json& j, RuleSets::EvaluateRules::Parameters& p) {
  get(p.geometry, j, "geometry");
  get(p.rulesets, j, "rulesets");
  if (j.count("flight_features") > 0) {
    auto ff = j["flight_features"];

    for (auto it = ff.begin(); it != ff.end(); ++it) {
      RuleSet::Feature::Value value = it.value();
      p.features.emplace(it.key(), value);
    }
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSets::EvaluateRules::Parameters& p) {
  j["geometry"] = p.geometry;
  j["rulesets"] = p.rulesets;

  for (const auto& pair : p.features) {
    j["flight_features"][pair.first] = pair.second;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSets::Search::Parameters& p) {
  j["geometry"] = p.geometry;
}
