
#ifndef AIRMAP_CODEC_JSON_STATUS_H_
#define AIRMAP_CODEC_JSON_STATUS_H_

#include <airmap/codec/json/airspace.h>
#include <airmap/status.h>

#include <airmap/codec/json/date_time.h>

#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

#include <string>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Status::Color& color) {
  std::stringstream ss{j.get<std::string>()};
  ss >> color;
}

inline void decode(const nlohmann::json& j, Status::Report& report) {
  get(report.max_safe_distance, j, "max_safe_distance");
  get(report.advisory_color, j, "advisory_color");
  get(report.advisories, j, "advisories");
  get(report.weather, j, "weather");
}

inline void decode(const nlohmann::json& j, Status::Advisory& advisory) {
  get(advisory.color, j, "color");
  decode(j, advisory.airspace);
}

inline void decode(const nlohmann::json& j, std::vector<Status::Advisory>& v) {
  for (auto element : j) {
    v.push_back(Status::Advisory{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Status::Weather& weather) {
  get(weather.condition, j, "condition");
  get(weather.condition, j, "icon");
  get(weather.wind, j, "wind");
  get(weather.humidity, j, "humidity");
  get(weather.visibility, j, "visibility");
  get(weather.precipitation, j, "precipitation");
  get(weather.temperature, j, "temperature");
}

inline void decode(const nlohmann::json& j, Status::Wind& wind) {
  get(wind.heading, j, "heading");
  get(wind.speed, j, "speed");
  get(wind.gusting, j, "gusting");
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_STATUS_H_
