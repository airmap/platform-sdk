#include <airmap/codec/json/advisory.h>

#include <airmap/codec.h>
#include <airmap/codec/json/airspace.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>
#include <airmap/codec/json/status.h>

void airmap::codec::json::decode(const nlohmann::json& j, Advisory::Advisory& advisory) {
  get(advisory.color, j, "color");
  decode(j, advisory.advisory);
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Advisory::Advisory>& v) {
  for (auto element : j) {
    v.push_back(Advisory::Advisory{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Advisory::Weather& weather) {
  get(weather.condition, j, "condition");
  get(weather.condition, j, "icon");
  get(weather.wind, j, "wind");
  get(weather.humidity, j, "humidity");
  get(weather.visibility, j, "visibility");
  get(weather.precipitation, j, "precipitation");
  get(weather.temperature, j, "temperature");
  get(weather.timezone, j, "timezone");
  get(weather.time, j, "time");
  get(weather.dew_point, j, "dew_point");
  get(weather.mslp, j, "mslp");
}

void airmap::codec::json::decode(const nlohmann::json& j, Advisory::Wind& wind) {
  get(wind.heading, j, "heading");
  get(wind.speed, j, "speed");
  get(wind.gusting, j, "gusting");
}