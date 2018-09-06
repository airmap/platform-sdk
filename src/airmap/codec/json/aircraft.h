//
//  aircraft.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_AIRCRAFT_H_
#define AIRMAP_CODEC_JSON_AIRCRAFT_H_

#include <airmap/aircraft.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Aircraft& aircraft);
void decode(const nlohmann::json& j, std::vector<Aircraft>& aircrafts);
void decode(const nlohmann::json& j, Aircraft::Manufacturer& manufacturer);
void decode(const nlohmann::json& j, std::vector<Aircraft::Manufacturer>& manufacturers);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AIRCRAFT_H_