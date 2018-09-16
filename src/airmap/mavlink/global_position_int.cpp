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
//  global_position_int.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/mavlink/global_position_int.h>

#include <iostream>

std::ostream& operator<<(std::ostream& out, const airmap::mavlink::GlobalPositionInt& position) {
  return out << "(" << position.time_boot_ms << " [ms]"
             << "," << position.lat / 1E7 << " [°]"
             << "," << position.lon / 1E7 << " [°]"
             << "," << position.alt / 1E3 << " [m]"
             << "," << position.relative_alt / 1E3 << " [m]"
             << "," << position.vx / 1E2 << " [m/s]"
             << "," << position.vy / 1E2 << " [m/s]"
             << "," << position.vz / 1E2 << " [m/s]"
             << "," << position.hdg / 1E2 << " [°]"
             << ")";
}