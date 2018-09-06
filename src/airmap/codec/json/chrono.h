//
//  chrono.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_CHRONO_H_
#define AIRMAP_CODEC_JSON_CHRONO_H_

#include <airmap/codec/json/get.h>

#include <nlohmann/json.hpp>

#include <chrono>

namespace std {

void from_json(const nlohmann::json& j, std::chrono::seconds& s);

}  // namespace std

#endif  // AIRMAP_CODEC_JSON_CHRONO_H_