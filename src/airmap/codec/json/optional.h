// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
