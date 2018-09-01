//
//  flight.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//


#ifndef AIRMAP_CODEC_JSON_FLIGHT_H_
#define AIRMAP_CODEC_JSON_FLIGHT_H_

#include <airmap/flight.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Flight& f);
void decode(const nlohmann::json& j, std::vector<Flight>& f);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHT_H_