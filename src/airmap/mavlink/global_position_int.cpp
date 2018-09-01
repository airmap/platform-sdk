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