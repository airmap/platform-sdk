
#ifndef AIRMAP_CODEC_JSON_RULESET_H_
#define AIRMAP_CODEC_JSON_RULESET_H_

#include <airmap/ruleset.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, RuleSet& ruleset);
void decode(const nlohmann::json& j, std::vector<RuleSet>& v);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULESET_H_
