//
//  geometry.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/geometry.h>

airmap::Geometry::Geometry() : type_{Type::invalid} {
}

airmap::Geometry::Geometry(const Point& other) : type_{Type::invalid} {
  set_point(other);
}

airmap::Geometry::Geometry(const MultiPoint& other) : type_{Type::invalid} {
  set_multi_point(other);
}

airmap::Geometry::Geometry(const LineString& other) : type_{Type::invalid} {
  set_line_string(other);
}

airmap::Geometry::Geometry(const MultiLineString& other) : type_{Type::invalid} {
  set_multi_line_string(other);
}

airmap::Geometry::Geometry(const Polygon& other) : type_{Type::invalid} {
  set_polygon(other);
}

airmap::Geometry::Geometry(const MultiPolygon& other) : type_{Type::invalid} {
  set_multi_polygon(other);
}

airmap::Geometry::Geometry(const GeometryCollection& other) : type_{Type::invalid} {
  set_geometry_collection(other);
}

airmap::Geometry::Geometry(const Geometry& other) : type_{Type::invalid} {
  set_geometry(other);
}

airmap::Geometry::~Geometry() {
  reset();
}

airmap::Geometry& airmap::Geometry::operator=(const Geometry& rhs) {
  return reset().set_geometry(rhs);
}

bool airmap::Geometry::operator==(const Geometry& rhs) const {
  if (type() != rhs.type())
    return false;

  switch (type()) {
    case Type::invalid:
      return true;
    case Type::point:
      return details_for_point() == rhs.details_for_point();
    case Type::multi_point:
      return details_for_multi_point() == rhs.details_for_multi_point();
    case Type::line_string:
      return details_for_line_string() == rhs.details_for_line_string();
    case Type::multi_line_string:
      return details_for_multi_line_string() == rhs.details_for_multi_line_string();
    case Type::polygon:
      return details_for_polygon() == rhs.details_for_polygon();
    case Type::multi_polygon:
      return details_for_multi_polygon() == rhs.details_for_multi_polygon();
    case Type::geometry_collection:
      return details_for_geometry_collection() == rhs.details_for_geometry_collection();
    default:
      break;
  }
  return false;
}

airmap::Geometry::Type airmap::Geometry::type() const {
  return type_;
}

const airmap::Geometry::Point& airmap::Geometry::details_for_point() const {
  return data_.point;
}

const airmap::Geometry::MultiPoint& airmap::Geometry::details_for_multi_point() const {
  return data_.multi_point;
}

const airmap::Geometry::LineString& airmap::Geometry::details_for_line_string() const {
  return data_.line_string;
}

const airmap::Geometry::MultiLineString& airmap::Geometry::details_for_multi_line_string() const {
  return data_.multi_line_string;
}

const airmap::Geometry::Polygon& airmap::Geometry::details_for_polygon() const {
  return data_.polygon;
}

const airmap::Geometry::MultiPolygon& airmap::Geometry::details_for_multi_polygon() const {
  return data_.multi_polygon;
}

const airmap::Geometry::GeometryCollection airmap::Geometry::details_for_geometry_collection() const {
  return data_.geometry_collection;
}

airmap::Geometry& airmap::Geometry::reset() {
  switch (type_) {
    case Type::point:
      data_.point.~Point();
      break;
    case Type::multi_point:
      data_.multi_point.~MultiPoint();
      break;
    case Type::line_string:
      data_.line_string.~LineString();
      break;
    case Type::multi_line_string:
      data_.multi_line_string.~MultiLineString();
      break;
    case Type::polygon:
      data_.polygon.~Polygon();
      break;
    case Type::multi_polygon:
      data_.multi_polygon.~MultiPolygon();
      break;
    case Type::geometry_collection:
      data_.geometry_collection.~GeometryCollection();
      break;
    default:
      break;
  }

  type_ = Type::invalid;
  return *this;
}

void airmap::Geometry::set_point(const Point& point) {
  type_ = Type::point;
  new (&data_.point) Point{point};
}

void airmap::Geometry::set_multi_point(const MultiPoint& multi_point) {
  type_ = Type::multi_point;
  new (&data_.multi_point) MultiPoint{multi_point};
}

void airmap::Geometry::set_line_string(const LineString& line_string) {
  type_ = Type::line_string;
  new (&data_.line_string) LineString{line_string};
}

void airmap::Geometry::set_multi_line_string(const MultiLineString& multi_line_string) {
  type_ = Type::multi_line_string;
  new (&data_.multi_line_string) MultiLineString{multi_line_string};
}

void airmap::Geometry::set_polygon(const Polygon& polygon) {
  type_ = Type::polygon;
  new (&data_.polygon) Polygon{polygon};
}

void airmap::Geometry::set_multi_polygon(const MultiPolygon& multi_polygon) {
  type_ = Type::multi_polygon;
  new (&data_.multi_polygon) MultiPolygon{multi_polygon};
}

void airmap::Geometry::set_geometry_collection(const GeometryCollection& geometry_collection) {
  type_ = Type::geometry_collection;
  new (&data_.geometry_collection) GeometryCollection{geometry_collection};
}

airmap::Geometry& airmap::Geometry::set_geometry(const Geometry& other) {
  type_ = other.type_;
  switch (type_) {
    case Type::invalid:
      break;
    case Type::point:
      set_point(other.details_for_point());
      break;
    case Type::multi_point:
      set_multi_point(other.details_for_multi_point());
      break;
    case Type::line_string:
      set_line_string(other.details_for_line_string());
      break;
    case Type::multi_line_string:
      set_multi_line_string(other.details_for_multi_line_string());
      break;
    case Type::polygon:
      set_polygon(other.details_for_polygon());
      break;
    case Type::multi_polygon:
      set_multi_polygon(other.details_for_multi_polygon());
      break;
    case Type::geometry_collection:
      set_geometry_collection(other.details_for_geometry_collection());
      break;
  }

  return *this;
}

airmap::Geometry::Data::Data() : invalid{} {
}
airmap::Geometry::Data::~Data() {
}

airmap::Geometry airmap::Geometry::point(double lat, double lon) {
  Geometry::Point p;
  p.latitude  = lat;
  p.longitude = lon;
  return Geometry{p};
}

airmap::Geometry airmap::Geometry::polygon(const std::vector<Coordinate>& coordinates) {
  Geometry::Polygon polygon{{coordinates}, {}};
  return Geometry{polygon};
}

bool airmap::operator==(const Geometry::Polygon& lhs, const Geometry::Polygon& rhs) {
  return lhs.outer_ring == rhs.outer_ring && lhs.inner_rings == rhs.inner_rings;
}

bool airmap::operator==(const Geometry::Coordinate& lhs, const Geometry::Coordinate& rhs) {
  return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude && lhs.altitude == rhs.altitude;
}
