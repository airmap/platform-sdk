#include <airmap/codec/http/query/advisories.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Advisory::ForId::Parameters& parameters) {
  if (parameters.start)
    query["start"] = iso8601::generate(parameters.start.get());
  if (parameters.end)
    query["end"] = iso8601::generate(parameters.end.get());
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Advisory::ReportWeather::Parameters& parameters) {
  if (parameters.latitude)
    query["latitude"] = boost::lexical_cast<std::string>(parameters.latitude);
  if (parameters.longitude)
    query["longitude"] = boost::lexical_cast<std::string>(parameters.longitude);
  if (parameters.start)
    query["start"] = iso8601::generate(parameters.start.get());
  if (parameters.end)
    query["end"] = iso8601::generate(parameters.end.get());
}