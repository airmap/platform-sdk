//
//  cheap_ruler.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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