//
//  aircraft.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/aircraft.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Aircraft& aircraft) {
  get(aircraft.model.id, j, "id");
  get(aircraft.model.name, j, "name");
  get(aircraft.manufacturer, j, "manufacturer");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Aircraft>& aircrafts) {
  for (const auto& element : j) {
    aircrafts.emplace_back();
    aircrafts.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Aircraft::Manufacturer& manufacturer) {
  get(manufacturer.id, j, "id");
  get(manufacturer.name, j, "name");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Aircraft::Manufacturer>& manufacturers) {
  for (const auto& element : j) {
    manufacturers.emplace_back();
    manufacturers.back() = element;
  }
}