//
//  pilots.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/pilots.h>

#include <iostream>
#include <type_traits>

namespace {

struct Comma {
  bool first = true;
};

std::ostream &operator<<(std::ostream &out, Comma &comma) {
  if (not comma.first)
    out << ",";
  comma.first = false;
  return out;
}

}  // namespace

airmap::Pilots::Exclude airmap::operator|(Pilots::Exclude lhs, Pilots::Exclude rhs) {
  using UT = typename std::underlying_type<Pilots::Exclude>::type;
  return static_cast<Pilots::Exclude>(static_cast<UT>(lhs) | static_cast<UT>(rhs));
}

airmap::Pilots::Exclude airmap::operator&(Pilots::Exclude lhs, Pilots::Exclude rhs) {
  using UT = typename std::underlying_type<Pilots::Exclude>::type;
  return static_cast<Pilots::Exclude>(static_cast<UT>(lhs) & static_cast<UT>(rhs));
}

std::ostream &airmap::operator<<(std::ostream &out, Pilots::Exclude exclude) {
  Comma comma;

  if ((exclude & Pilots::Exclude::aircraft) == Pilots::Exclude::aircraft)
    out << comma << "aircraft";
  if ((exclude & Pilots::Exclude::user_metadata) == Pilots::Exclude::user_metadata)
    out << comma << "user_metadata";
  if ((exclude & Pilots::Exclude::app_metadata) == Pilots::Exclude::app_metadata)
    out << comma << "app_metadata";
  if ((exclude & Pilots::Exclude::authorized_apps) == Pilots::Exclude::authorized_apps)
    out << comma << "authorized_apps";

  return out;
}