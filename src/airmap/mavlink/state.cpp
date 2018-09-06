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