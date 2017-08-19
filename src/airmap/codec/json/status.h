
#ifndef AIRMAP_CODEC_JSON_STATUS_H_
#define AIRMAP_CODEC_JSON_STATUS_H_

#include <airmap/codec/json/airspace.h>
#include <airmap/status.h>

#include <airmap/codec/json/date_time.h>

#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

#include <iostream>

#include <string>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Status::Color& color) {
  const auto& color_string = j.get<std::string>();

  if (color_string == "green")
    color = Status::Color::green;
  else if (color_string == "yellow")
    color = Status::Color::yellow;
  else if (color_string == "orange")
    color = Status::Color::orange;
  else if (color_string == "red")
    color = Status::Color::red;
}

inline void decode(const nlohmann::json& j, Status::Report& report) {
  get(report.max_safe_distance, j, "max_safe_distance");
  get(report.advisory_color, j, "advisory_color");
  get(report.advisories, j, "advisories");
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

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_STATUS_H_
