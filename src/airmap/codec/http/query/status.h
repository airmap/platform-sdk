#ifndef AIRMAP_CODEC_HTTP_QUERY_STATUS_H_
#define AIRMAP_CODEC_HTTP_QUERY_STATUS_H_

#include <airmap/status.h>

#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

#include <sstream>
#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Status::GetStatus::Parameters& parameters) {
  if (parameters.latitude)
    query["latitude"] = boost::lexical_cast<std::string>(parameters.latitude);
  if (parameters.longitude)
    query["longitude"] = boost::lexical_cast<std::string>(parameters.longitude);
  if (parameters.types != Airspace::Type::invalid) {
    query["types"] = boost::lexical_cast<std::string>((parameters.types & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
  }
  if (parameters.ignored_types && parameters.ignored_types.get() != Airspace::Type::invalid) {
    query["ignored_types"] = boost::lexical_cast<std::string>((parameters.ignored_types.get() & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
  }
  if (parameters.weather)
    query["weather"] = parameters.weather ? "true" : "false";
  if (parameters.flight_date_time)
    query["datetime"] = iso8601::generate(parameters.flight_date_time.get());
  if (parameters.geometry) {
    nlohmann::json geometry;
    geometry          = parameters.geometry.get();
    query["geometry"] = geometry.dump();
  }
  if (parameters.buffer)
    query["buffer"] = boost::lexical_cast<std::string>(parameters.buffer.get());
}

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_STATUS_H_
