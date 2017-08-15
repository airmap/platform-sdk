#ifndef AIRMAP_CODEC_HTTP_QUERY_PILOTS_H_
#define AIRMAP_CODEC_HTTP_QUERY_PILOTS_H_

#include <airmap/pilots.h>

#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

#include <sstream>
#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Pilots::Authenticated::Parameters& parameters) {
  if (parameters.exclude)
    query["enhance"]  = boost::lexical_cast<std::string>(parameters.exclude.get());
  query["statistics"] = parameters.retrieve_statistics ? "true" : "false";
}

inline void encode(std::unordered_map<std::string, std::string>& query, const Pilots::ForId::Parameters& parameters) {
  if (parameters.exclude)
    query["enhance"]  = boost::lexical_cast<std::string>(parameters.exclude.get());
  query["statistics"] = parameters.retrieve_statistics ? "true" : "false";
}

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_PILOTS_H_
