
#ifndef AIRMAP_CODEC_JSON_TRAFFIC_H_
#define AIRMAP_CODEC_JSON_TRAFFIC_H_

#include <airmap/traffic.h>

#include <airmap/codec/json/get.h>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Traffic::Update& update) {
  get(update.id, j, "id");
  get(update.aircraft_id, j, "aircraft_id");
  get(update.direction, j, "direction");
  get(update.latitude, j, "latitude");
  get(update.longitude, j, "longitude");
  get(update.altitude, j, "altitude");
  get(update.ground_speed, j, "ground_speed_kts");
  get(update.heading, j, "true_heading");
  get(update.timestamp, j, "recorded");
  get(update.timestamp, j, "timestamp");

  update.altitude *= 0.3048;
  update.ground_speed *= 0.514444;
}

inline void decode(const nlohmann::json& j, std::vector<Traffic::Update>& v) {
  for (auto element : j) {
    v.push_back(Traffic::Update{});
    v.back() = element;
  }
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_TRAFFIC_H_
