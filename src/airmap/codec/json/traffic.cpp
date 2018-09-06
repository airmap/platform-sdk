//
//  traffic.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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
    update.altitude = boost::lexical_cast<double>(numeric_value_as_string);

    get(numeric_value_as_string, j, "ground_speed_kts");
    update.ground_speed = boost::lexical_cast<double>(numeric_value_as_string);

    get(numeric_value_as_string, j, "true_heading");
    update.heading = boost::lexical_cast<double>(numeric_value_as_string);
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
