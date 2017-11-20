#include <airmap/codec/http/query/pilots.h>

#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Pilots::Authenticated::Parameters& parameters) {
  if (parameters.exclude)
    query["enhance"]  = boost::lexical_cast<std::string>(parameters.exclude.get());
  query["statistics"] = parameters.retrieve_statistics ? "true" : "false";
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Pilots::ForId::Parameters& parameters) {
  if (parameters.exclude)
    query["enhance"]  = boost::lexical_cast<std::string>(parameters.exclude.get());
  query["statistics"] = parameters.retrieve_statistics ? "true" : "false";
}
