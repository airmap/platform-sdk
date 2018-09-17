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
#include <airmap/codec/json/rule.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Rule& rule) {
  get(rule.type, j, "type");
  get(rule.id, j, "id");
  get(rule.name, j, "name");
  get(rule.description, j, "description");
  get(rule.jurisdiction, j, "jurisdiction");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Rule>& v) {
  for (auto element : j) {
    v.push_back(Rule{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json&, Rule::Type&) {
}
