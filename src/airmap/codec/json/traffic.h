
#ifndef AIRMAP_CODEC_JSON_TRAFFIC_H_
#define AIRMAP_CODEC_JSON_TRAFFIC_H_

#include <airmap/traffic.h>

#include <airmap/codec/json/get.h>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Traffic::TrafficInfo& info) {
  get(info.TBD, j, "TBD");
}

inline void encode(nlohmann::json& j, const Traffic::Subscribe::Params& params) {
  j["flight_id"] = params.flight_id;
  j["token"]     = params.token;
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_TRAFFIC_H_
