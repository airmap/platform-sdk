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
#include <airmap/codec/json/traffic.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Traffic::Update& update) {
  get(update.id, j, "id");
  get(update.aircraft_id, j["properties"], "aircraft_id");
  get(update.direction, j, "direction");

  {
    // The fun begins here. Actual values are not numbers in the json
    // but rather encoded as string to address an issue with floating
    // point resolution. For that, we grab numeric values as strings and
    // convert them to floating point after that.
    std::string numeric_value_as_string;

    get(numeric_value_as_string, j, "latitude");
    update.latitude = boost::lexical_cast<double>(numeric_value_as_string);

    get(numeric_value_as_string, j, "longitude");
    update.longitude = boost::lexical_cast<double>(numeric_value_as_string);

    get(numeric_value_as_string, j, "altitude");
    boost::conversion::try_lexical_convert(numeric_value_as_string, update.altitude);

    get(numeric_value_as_string, j, "ground_speed_kts");
    boost::conversion::try_lexical_convert(numeric_value_as_string, update.ground_speed);

    get(numeric_value_as_string, j, "true_heading");
    boost::conversion::try_lexical_convert(numeric_value_as_string, update.heading);
  }

  std::string tss;
  get(tss, j, "recorded_time");
  update.recorded = from_seconds_since_epoch(seconds(boost::lexical_cast<std::int64_t>(tss)));

  std::int64_t ts;
  get(ts, j, "timestamp");
  update.timestamp = from_milliseconds_since_epoch(milliseconds(ts));

  update.altitude *= 0.3048;
  update.ground_speed *= 0.514444;
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Traffic::Update>& v) {
  for (auto element : j) {
    v.push_back(Traffic::Update{});
    v.back() = element;
  }
}
