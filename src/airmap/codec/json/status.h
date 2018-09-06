//
//  status.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_STATUS_H_
#define AIRMAP_CODEC_JSON_STATUS_H_

#include <airmap/status.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Status::Color& color);
void decode(const nlohmann::json& j, Status::Report& report);
void decode(const nlohmann::json& j, Status::Advisory& advisory);
void decode(const nlohmann::json& j, std::vector<Status::Advisory>& v);
void decode(const nlohmann::json& j, Status::Weather& weather);
void decode(const nlohmann::json& j, Status::Wind& wind);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_STATUS_H_
