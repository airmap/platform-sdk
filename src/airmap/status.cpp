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
//  status.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/status.h>

#include <iosfwd>

std::ostream& airmap::operator<<(std::ostream& out, Status::Color color) {
  switch (color) {
    case Status::Color::green:
      out << "green";
      break;
    case Status::Color::yellow:
      out << "yellow";
      break;
    case Status::Color::orange:
      out << "orange";
      break;
    case Status::Color::red:
      out << "red";
      break;
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, Status::Color& color) {
  std::string s;
  in >> s;

  if (s == "green")
    color = Status::Color::green;
  else if (s == "yellow")
    color = Status::Color::yellow;
  else if (s == "orange")
    color = Status::Color::orange;
  else if (s == "red")
    color = Status::Color::red;

  return in;
}