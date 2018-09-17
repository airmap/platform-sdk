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
#ifndef AIRMAP_CODEC_JSON_TRAFFIC_H_
#define AIRMAP_CODEC_JSON_TRAFFIC_H_

#include <airmap/traffic.h>

#include <nlohmann/json.hpp>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Traffic::Update& update);
void decode(const nlohmann::json& j, std::vector<Traffic::Update>& v);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_TRAFFIC_H_
