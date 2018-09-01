//
//  telemetry_simulator.h
//  Airmap Platform SDK
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
