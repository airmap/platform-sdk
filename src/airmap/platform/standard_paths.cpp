//
//  standard_paths.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/platform/standard_paths.h>

#include <iostream>

std::ostream& airmap::platform::operator<<(std::ostream& out, StandardPaths::Scope scope) {
  switch (scope) {
    case StandardPaths::Scope::system:
      return out << "system";
    case StandardPaths::Scope::user:
      return out << "user";
  }

  return out;
}

std::ostream& airmap::platform::operator<<(std::ostream& out, StandardPaths::Location location) {
  switch (location) {
    case StandardPaths::Location::cache:
      return out << "cache";
    case StandardPaths::Location::config:
      return out << "config";
    case StandardPaths::Location::data:
      return out << "data";
    case StandardPaths::Location::runtime:
      return out << "runtime";
  }

  return out;
}
