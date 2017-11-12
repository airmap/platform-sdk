#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE airspace

#include <airmap/airspace.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(default_ctor_yields_an_invalid_airspace) {
  airmap::Airspace airspace;
  BOOST_CHECK(airspace.type() == airmap::Airspace::Type::invalid);
}

BOOST_AUTO_TEST_CASE(copy_ctor_yields_instances_that_compare_equal) {
  airmap::Airspace a1, a2;
  BOOST_CHECK(a1 == a2);
}

BOOST_AUTO_TEST_CASE(assignment_operator_yields_instances_that_compare_equal_for_default_ctor) {
  airmap::Airspace a1;
  auto a2 = a1;
  BOOST_CHECK(a1 == a2);
}

BOOST_AUTO_TEST_CASE(assignment_operator_yields_instances_that_compare_equal_for_instances_with_trivial_details) {
  airmap::Airspace::Park park;
  airmap::Airspace a1;
  a1.set_details(park);
  auto a2 = a1;
  BOOST_CHECK(a1 == a2);
}

BOOST_AUTO_TEST_CASE(assignment_operator_yields_instances_that_compare_equal_for_instances_with_complex_details) {
  airmap::Airspace::Airport airport;
  airmap::Airspace a1;
  a1.set_details(airport);
  auto a2 = a1;
  BOOST_CHECK(a1 == a2);
}

BOOST_AUTO_TEST_CASE(instances_with_differences_do_not_compare_equal_for_different_properties) {
  airmap::Airspace a1, a2;
  a1.set_name("a1");
  a2.set_name("a2");
  BOOST_CHECK(a1 != a2);
}

BOOST_AUTO_TEST_CASE(instances_with_differences_do_not_compare_equal_for_different_geometries) {
  airmap::Airspace a1, a2;

  airmap::Geometry::Point p;
  p.latitude  = 42.f;
  p.longitude = 21.f;
  p.altitude  = 6.f;

  airmap::Geometry::LineString ls;
  ls.coordinates.push_back(airmap::Geometry::Coordinate{42.f, 21.f, 6.f, 6.f});
  ls.coordinates.push_back(airmap::Geometry::Coordinate{41.f, 21.f, 6.f, 6.f});

  a1.set_geometry(airmap::Geometry{p});
  a2.set_geometry(airmap::Geometry{ls});

  BOOST_CHECK(a1 != a2);
}
