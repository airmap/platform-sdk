// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <airmap/util/telemetry_simulator.h>

#include <algorithm>
#include <iostream>

airmap::util::TelemetrySimulator::TelemetrySimulator(const airmap::Geometry::Polygon& polygon, double velocity)
    : velocity_{velocity},
      polygon_{polygon},
      segment_begin_{polygon_.outer_ring.coordinates.begin()},
      segment_end_{polygon_.outer_ring.coordinates.begin() + 1},
      segment_ruler_{segment_begin_->latitude},
      segment_start_time_{Clock::universal_time()},
      segment_bearing_{segment_ruler_.bearing(*segment_begin_, *segment_end_)},
      segment_length_{segment_ruler_.distance(*segment_begin_, *segment_end_)},
      current_data_{*segment_begin_},
      current_time_{Clock::universal_time()} {
}

airmap::util::TelemetrySimulator::TelemetrySimulator(const TelemetrySimulator& other)
    : velocity_{other.velocity_},
      polygon_{other.polygon_},
      segment_begin_{polygon_.outer_ring.coordinates.begin() +
                     std::distance(other.polygon_.outer_ring.coordinates.begin(), other.segment_begin_)},
      segment_end_{polygon_.outer_ring.coordinates.begin() +
                   std::distance(other.polygon_.outer_ring.coordinates.begin(), other.segment_end_)},
      segment_ruler_{other.segment_ruler_},
      segment_start_time_{other.segment_start_time_},
      segment_bearing_{other.segment_bearing_},
      segment_length_{other.segment_length_},
      current_data_{other.current_data_},
      current_time_{other.current_time_} {
}

airmap::util::TelemetrySimulator& airmap::util::TelemetrySimulator::operator=(const TelemetrySimulator& other) {
  velocity_      = other.velocity_;
  polygon_       = other.polygon_;
  segment_begin_ = polygon_.outer_ring.coordinates.begin() +
                   std::distance(other.polygon_.outer_ring.coordinates.begin(), other.segment_begin_);
  segment_end_ = polygon_.outer_ring.coordinates.begin() +
                 std::distance(other.polygon_.outer_ring.coordinates.begin(), other.segment_end_);
  segment_ruler_      = other.segment_ruler_;
  segment_start_time_ = other.segment_start_time_;
  segment_bearing_    = other.segment_bearing_;
  segment_length_     = other.segment_length_;
  current_data_       = other.current_data_;
  current_time_       = other.current_time_;
  return *this;
}

airmap::Geometry::Coordinate airmap::util::TelemetrySimulator::update(const DateTime& now) {
  if (segment_ruler_.distance(*segment_begin_, current_data_) > segment_length_) {
    segment_begin_ = segment_end_;
    segment_end_   = segment_end_ + 1;

    if (segment_end_ == polygon_.outer_ring.coordinates.end()) {
      segment_begin_ = polygon_.outer_ring.coordinates.begin();
      segment_end_   = segment_begin_ + 1;
    }

    segment_ruler_      = CheapRuler{segment_begin_->latitude};
    segment_length_     = segment_ruler_.distance(*segment_begin_, *segment_end_);
    segment_bearing_    = segment_ruler_.bearing(*segment_begin_, *segment_end_);
    segment_start_time_ = Clock::universal_time();
    current_data_       = *segment_begin_;
  }

  auto dt       = now - current_time_;
  current_data_ = segment_ruler_.destination(current_data_, dt.total_milliseconds() / 1E3 * velocity_, segment_bearing_);
  current_time_ = now;

  return current_data_;
}
