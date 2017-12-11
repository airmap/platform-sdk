#define BOOST_TEST_MODULE geometry

#include <airmap/geometry.h>

#include <boost/test/unit_test.hpp>

namespace airmap {

std::ostream& operator<<(std::ostream& out, Geometry::Type) {
  return out;
}

std::ostream& operator<<(std::ostream& out, const Geometry&) {
  return out;
}

}  // namespace airmap

BOOST_AUTO_TEST_CASE(default_ctor_yields_an_invalid_geometry) {
  airmap::Geometry geometry;
  BOOST_CHECK(geometry.type() == airmap::Geometry::Type::invalid);
}

BOOST_AUTO_TEST_CASE(copy_ctor_yields_instances_that_compare_equal) {
  airmap::Geometry g1, g2;
  BOOST_CHECK(g1 == g2);
}

BOOST_AUTO_TEST_CASE(assignment_operator_yields_instances_that_compare_equal_for_default_ctor) {
  airmap::Geometry g1;
  auto g2 = g1;
  BOOST_CHECK(g1 == g2);
}

BOOST_AUTO_TEST_CASE(assignment_operator_yields_instances_that_compare_equal_for_instances_with_trivial_details) {
  airmap::Geometry::Point p;
  p.latitude  = 42.f;
  p.longitude = 21.f;
  p.altitude  = 6.f;

  airmap::Geometry g1{p};
  auto g2 = g1;
  BOOST_CHECK(g1 == g2);
}

BOOST_AUTO_TEST_CASE(assignment_operator_yields_instances_that_compare_equal_for_instances_with_complex_details) {
  airmap::Geometry::GeometryCollection gc;

  airmap::Geometry::Point p;
  p.latitude  = 42.f;
  p.longitude = 21.f;
  p.altitude  = 6.f;

  airmap::Geometry::LineString ls;
  ls.coordinates.push_back(airmap::Geometry::Coordinate{42.f, 21.f, 6.f, 6.f});
  ls.coordinates.push_back(airmap::Geometry::Coordinate{41.f, 21.f, 6.f, 6.f});

  gc.push_back(airmap::Geometry{p});
  gc.push_back(airmap::Geometry{ls});

  airmap::Geometry g1{gc};
  auto g2 = g1;
  BOOST_CHECK(g1 == g2);
}
