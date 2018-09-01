//
//  geometry.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//


#ifndef AIRMAP_CODEC_JSON_GEOMETRY_H_
#define AIRMAP_CODEC_JSON_GEOMETRY_H_

#include <airmap/codec/json/get.h>
#include <airmap/geometry.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Geometry& g);
void decode(const nlohmann::json& j, std::vector<Geometry>& v);
void decode(const nlohmann::json& j, Geometry::Type& t);
void decode(const nlohmann::json& j, Geometry::Coordinate& c);

void decode(const nlohmann::json& j, Geometry::MultiLineString& mls);
void decode(const nlohmann::json& j, Geometry::Polygon& p);
void decode(const nlohmann::json& j, Geometry::MultiPolygon& mp);
void encode(nlohmann::json& j, const Geometry& geometry);
void encode(nlohmann::json& j, const std::vector<Geometry>& geometries);
void encode(nlohmann::json& j, Geometry::Type type);
void encode(nlohmann::json& j, const Geometry::Coordinate& coordinate);
void encode(nlohmann::json& j, const std::vector<Geometry::Coordinate>& coordinates);
void encode(nlohmann::json& j, const Geometry::Polygon& cvs);
void encode(nlohmann::json& j, const std::vector<Geometry::Polygon>& cvs);

template <Geometry::Type tag>
inline void encode(nlohmann::json& j, const Geometry::CoordinateVector<tag>& cv) {
  j = cv.coordinates;
}

template <Geometry::Type tag>
inline void encode(nlohmann::json& j, const std::vector<Geometry::CoordinateVector<tag>>& cvs) {
  for (const auto& cv : cvs)
    j.push_back(cv);
}

template <Geometry::Type tag>
inline void decode(const nlohmann::json& j, Geometry::CoordinateVector<tag>& cv) {
  for (const auto& element : j) {
    cv.coordinates.push_back(Geometry::Point{});
    cv.coordinates.back() = element;
  }
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_GEOMETRY_H_
