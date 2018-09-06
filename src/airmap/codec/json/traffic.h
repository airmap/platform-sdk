//
//  traffic.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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
