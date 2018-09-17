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
#ifndef AIRMAP_UTIL_TAGGED_STRING_H_
#define AIRMAP_UTIL_TAGGED_STRING_H_

#include <string>

namespace airmap {
namespace util {

template <typename Tag>
class TaggedString {
 public:
  explicit TaggedString() {
  }
  explicit TaggedString(const std::string& value) : value_{value} {
  }
  explicit TaggedString(const char* s) : value_{s} {
  }

  operator const std::string&() const {
    return value_;
  }

  std::string& string() {
    return value_;
  }

  const std::string& string() const {
    return value_;
  }

  bool validate() const {
    return Tag::validate(value_);
  }

 private:
  std::string value_;
};

template <typename Tag>
std::istream& operator>>(std::istream& in, TaggedString<Tag>& ts) {
  return in >> ts.string();
}

template <typename Tag>
std::ostream& operator<<(std::ostream& out, const TaggedString<Tag>& ts) {
  return out << ts.string();
}

namespace tags {

struct MustNotBeEmpty {
  static bool validate(const std::string& s) {
    return !s.empty();
  }
};

}  // namespace tags

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_TAGGED_STRING_H_
