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
//  telemetry_simulator.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_UTIL_TELEMETRY_SIMULATOR_H_
#define AIRMAP_UTIL_TELEMETRY_SIMULATOR_H_

#include <airmap/date_time.h>
#include <airmap/geometry.h>
#include <airmap/util/cheap_ruler.h>

#include <chrono>

namespace airmap {
namespace util {

class TelemetrySimulator {
 public:
  explicit TelemetrySimulator(const airmap::Geometry::Polygon& polygon);
  TelemetrySimulator(const TelemetrySimulator& other);
  TelemetrySimulator& operator=(const TelemetrySimulator& rhs);
  // Updates the state of the simulator and calculates a new position.
  Geometry::Coordinate update(const DateTime& now = airmap::Clock::universal_time());

 private:
  airmap::Geometry::Polygon polygon_;
  std::vector<airmap::Geometry::Coordinate>::const_iterator segment_begin_;
  std::vector<airmap::Geometry::Coordinate>::const_iterator segment_end_;
  CheapRuler segment_ruler_;
  DateTime segment_start_time_;
  double segment_bearing_;
  double segment_length_;
  Geometry::Coordinate current_data_;
  DateTime current_time_;
};

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_TELEMETRY_SIMULATOR_H_
