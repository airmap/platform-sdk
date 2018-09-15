/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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
