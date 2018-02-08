#ifndef AIRMAP_CODEC_HTTP_QUERY_ADVISORY_H_
#define AIRMAP_CODEC_HTTP_QUERY_ADVISORY_H_

#include <airmap/advisory.h>

#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

void encode(std::unordered_map<std::string, std::string>& query, const Advisory::ForId::Parameters& parameters);
void encode(std::unordered_map<std::string, std::string>& query, const Advisory::ReportWeather::Parameters& parameters);

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_ADVISORY_H_
