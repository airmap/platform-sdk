//
//  optional.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_OPTIONAL_H_
#define AIRMAP_CODEC_JSON_OPTIONAL_H_

#include <airmap/optional.h>

#include <nlohmann/json.hpp>

namespace airmap {
namespace codec {
namespace json {

template <typename T>
inline void decode(const nlohmann::json& j, Optional<T>& o) {
  if (j.is_null())
    o.reset();
  else
    o = j.get<T>();
}

template <typename T>
inline void encode(nlohmann::json& j, const Optional<T>& o) {
  if (o)
    j = o.get();
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_OPTIONAL_H_