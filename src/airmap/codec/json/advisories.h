//
//  advisories.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_ADVISORIES_H_
#define AIRMAP_CODEC_JSON_ADVISORIES_H_

#include <airmap/advisory.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void encode(nlohmann::json& j, const Advisory::Search::Parameters& p);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_ADVISORIES_H_
