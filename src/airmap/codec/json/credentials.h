
#ifndef AIRMAP_CODEC_JSON_CREDENTIALS_H_
#define AIRMAP_CODEC_JSON_CREDENTIALS_H_

#include <airmap/credentials.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Credentials::Anonymous& anonymous);
void decode(const nlohmann::json& j, Credentials::OAuth& oauth);
void decode(const nlohmann::json& j, Credentials& credentials);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_CREDENTIALS_H_
