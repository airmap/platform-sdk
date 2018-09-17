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
#include <airmap/codec/json/aircraft.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Aircraft& aircraft) {
  get(aircraft.model.id, j, "id");
  get(aircraft.model.name, j, "name");
  get(aircraft.manufacturer, j, "manufacturer");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Aircraft>& aircrafts) {
  for (const auto& element : j) {
    aircrafts.emplace_back();
    aircrafts.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Aircraft::Manufacturer& manufacturer) {
  get(manufacturer.id, j, "id");
  get(manufacturer.name, j, "name");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Aircraft::Manufacturer>& manufacturers) {
  for (const auto& element : j) {
    manufacturers.emplace_back();
    manufacturers.back() = element;
  }
}
