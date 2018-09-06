//
//  advisory.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_ADVISORY_H_
#define AIRMAP_CODEC_JSON_ADVISORY_H_

#include <airmap/advisory.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Advisory::AirspaceAdvisory& advisory);
void decode(const nlohmann::json& j, std::vector<Advisory::AirspaceAdvisory>& v);
void decode(const nlohmann::json& j, Advisory::Weather& weather);
void decode(const nlohmann::json& j, Advisory::Wind& wind);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_ADVISORY_H_
