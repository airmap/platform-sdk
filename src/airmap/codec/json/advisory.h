
#ifndef AIRMAP_CODEC_JSON_ADVISORY_H_
#define AIRMAP_CODEC_JSON_ADVISORY_H_

#include <airmap/advisory.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Status::Advisory& advisory);
void decode(const nlohmann::json& j, std::vector<Advisory::Advisory>& v);
void decode(const nlohmann::json& j, Advisory::Weather& weather);
void decode(const nlohmann::json& j, Advisory::Wind& wind);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_ADVISORY_H_
