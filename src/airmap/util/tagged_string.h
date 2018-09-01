//
//  tagged_string.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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