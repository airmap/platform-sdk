//
//  pilot.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_PILOT_H_
#define AIRMAP_CODEC_JSON_PILOT_H_

#include <airmap/pilot.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Pilot& p);
void decode(const nlohmann::json& j, Pilot::VerificationStatus& s);
void decode(const nlohmann::json& j, Pilot::Statistics& s);
void decode(const nlohmann::json& j, Pilot::Statistics::Flight& f);
void decode(const nlohmann::json& j, Pilot::Statistics::Aircraft& a);
void decode(const nlohmann::json& j, Pilot::Aircraft& a);
void decode(const nlohmann::json& j, std::vector<Pilot::Aircraft>& v);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_PILOT_H_