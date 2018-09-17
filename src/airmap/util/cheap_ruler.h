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
#ifndef AIRMAP_UTIL_CHEAP_RULER_H_
#define AIRMAP_UTIL_CHEAP_RULER_H_

#include <airmap/geometry.h>

#include <cmath>

namespace airmap {
namespace util {

class CheapRuler {
 public:
  CheapRuler(double latitude);

  double bearing(const Geometry::Coordinate& p1, const Geometry::Coordinate& p2) const;
  double distance(const Geometry::Coordinate& p1, const Geometry::Coordinate& p2) const;
  Geometry::Coordinate destination(const Geometry::Coordinate& c, double distance, double bearing) const;

 private:
  double kx_{0.f};
  double ky_{0.f};
};

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_CHEAP_RULER_H_
