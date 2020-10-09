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
#include <airmap/codec/json/elevation.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::decode(const nlohmann::json& j, Elevation::Carpet& carpet) {
  get(carpet.bounds, j, "bounds");
  get(carpet.stats, j, "stats");
  get(carpet.carpet, j, "carpet");
}

void airmap::codec::json::decode(const nlohmann::json& j, Elevation::Path& path) {
  get(path.from, j, "from");
  get(path.to, j, "to");
  get(path.step, j, "step");
  get(path.profile, j, "profile");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Elevation::Path>& v) {
  for (auto element : j) {
    v.push_back(Elevation::Path{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Elevation::Bounds& bounds) {
  get(bounds.sw, j, "sw");
  get(bounds.ne, j, "ne");
}

void airmap::codec::json::decode(const nlohmann::json& j, Elevation::Stats& stats) {
  get(stats.max, j, "max");
  get(stats.min, j, "min");
  get(stats.avg, j, "avg");
}
