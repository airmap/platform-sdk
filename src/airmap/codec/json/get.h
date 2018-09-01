//
//  get.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_GET_H_
#define AIRMAP_CODEC_JSON_GET_H_

#include <nlohmann/json.hpp>

namespace airmap {
namespace codec {
namespace json {

template <typename T>
inline void get(T& to, const nlohmann::json& from) {
  if (!from.is_null())
    to = from.get<T>();
}

template <typename T>
inline void get(T& to, const nlohmann::json& from, const char* name) {
  if (from.count(name) > 0)
    get(to, from.at(name));
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_GET_H_