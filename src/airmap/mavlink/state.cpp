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
//  state.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/mavlink/state.h>

#include <iostream>

std::ostream& operator<<(std::ostream& out, airmap::mavlink::State state) {
  switch (state) {
    case MAV_STATE_UNINIT:
      return out << "uninit";
    case MAV_STATE_BOOT:
      return out << "boot";
    case MAV_STATE_CALIBRATING:
      return out << "calibrating";
    case MAV_STATE_STANDBY:
      return out << "standby";
    case MAV_STATE_ACTIVE:
      return out << "active";
    case MAV_STATE_CRITICAL:
      return out << "critical";
    case MAV_STATE_EMERGENCY:
      return out << "emergency";
    case MAV_STATE_POWEROFF:
      return out << "poweroff";
    default:
      break;
  }

  return out;
}