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
#ifndef AIRMAP_UTIL_FMT_H_
#define AIRMAP_UTIL_FMT_H_

#include <boost/format.hpp>

#include <string>

namespace airmap {
namespace util {
namespace fmt {

template <typename Head>
std::string sprintf(boost::format& fmt, Head&& head) {
  return (fmt % head).str();
}

template <typename Head, typename... Tail>
std::string sprintf(boost::format& fmt, Head&& head, Tail&&... tail) {
  return sprintf(fmt % head, std::forward<Tail>(tail)...);
}

template <typename... Args>
inline std::string sprintf(const char* format, Args&&... args) {
  boost::format fmt{format};
  return sprintf(fmt, std::forward<Args>(args)...);
}

inline std::string sprintf(const char* format) {
  return format;
}

}  // namespace fmt
}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_FMT_H_
