//
//  cheap_ruler.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/util/cheap_ruler.h>

airmap::util::CheapRuler::CheapRuler(double latitude) {
  auto cos  = std::cos(std::floor(latitude) * M_PI / 180);
  auto cos2 = 2. * cos * cos - 1.;
  auto cos3 = 2. * cos * cos2 - cos;
  auto cos4 = 2. * cos * cos3 - cos2;
  auto cos5 = 2. * cos * cos4 - cos3;

  kx_ = 1000. * (111.41513 * cos - 0.09455 * cos3 + 0.00012 * cos5);
  ky_ = 1000. * (111.13209 - 0.56605 * cos2 + 0.0012 * cos4);
}

double airmap::util::CheapRuler::bearing(const airmap::Geometry::Coordinate& p1,
                                         const airmap::Geometry::Coordinate& p2) const {
  auto dx = (p2.longitude - p1.longitude) * kx_;
  auto dy = (p2.latitude - p1.latitude) * ky_;

  if (!(dx < 0 || dx > 0))
    return 0.f;
  if (!(dy < 0 || dy > 0))
    return 0.f;

  auto bearing = std::atan2(-dy, dx) * 180 / M_PI + 90;
  return bearing > 180 ? bearing -= 360 : bearing;
}

double airmap::util::CheapRuler::distance(const airmap::Geometry::Coordinate& p1,
                                          const airmap::Geometry::Coordinate& p2) const {
  auto dx = (p2.longitude - p1.longitude) * kx_;
  auto dy = (p2.latitude - p1.latitude) * ky_;
  return std::sqrt(dx * dx + dy * dy);
}

airmap::Geometry::Coordinate airmap::util::CheapRuler::destination(const airmap::Geometry::Coordinate& c,
                                                                   double distance, double bearing) const {
  auto a      = (90 - bearing) * M_PI / 180;
  auto result = c;

  result.longitude += (std::cos(a) * distance) / kx_;
  result.latitude += (std::sin(a) * distance) / ky_;

  return result;
}