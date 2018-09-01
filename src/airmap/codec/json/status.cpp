//
//  status.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/status.h>

#include <airmap/codec.h>
#include <airmap/codec/json/airspace.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::decode(const nlohmann::json& j, Status::Color& color) {
  std::stringstream ss{j.get<std::string>()};
  ss >> color;
}

void airmap::codec::json::decode(const nlohmann::json& j, Status::Report& report) {
  get(report.max_safe_distance, j, "max_safe_distance");
  get(report.advisory_color, j, "advisory_color");
  get(report.advisories, j, "advisories");
  get(report.weather, j, "weather");
}

void airmap::codec::json::decode(const nlohmann::json& j, Status::Advisory& advisory) {
  get(advisory.color, j, "color");
  decode(j, advisory.airspace);
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Status::Advisory>& v) {
  for (auto element : j) {
    v.push_back(Status::Advisory{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Status::Weather& weather) {
  get(weather.condition, j, "condition");
  get(weather.icon, j, "icon");
  get(weather.wind, j, "wind");
  get(weather.humidity, j, "humidity");
  get(weather.visibility, j, "visibility");
  get(weather.precipitation, j, "precipitation");
  get(weather.temperature, j, "temperature");
}

void airmap::codec::json::decode(const nlohmann::json& j, Status::Wind& wind) {
  get(wind.heading, j, "heading");
  get(wind.speed, j, "speed");
  get(wind.gusting, j, "gusting");
}