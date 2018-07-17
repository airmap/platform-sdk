
#ifndef AIRMAP_CODEC_JSON_DATE_TIME_H_
#define AIRMAP_CODEC_JSON_DATE_TIME_H_

#include <airmap/date_time.h>

#include <boost/date_time.hpp>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {

void from_json(const nlohmann::json& j, airmap::DateTime& dt);
void to_json(nlohmann::json& j, const airmap::DateTime& dt);

}  // namespace airmap

namespace boost {
namespace posix_time {

void from_json(const nlohmann::json& j, ptime& dt);
void to_json(nlohmann::json& j, const ptime& dt);

}  // namespace posix_time
}  // namespace boost

#endif  // AIRMAP_CODEC_JSON_DATE_TIME_H_
