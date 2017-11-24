
#ifndef AIRMAP_CODEC_JSON_RULESETS_H_
#define AIRMAP_CODEC_JSON_RULESETS_H_

#include <airmap/rulesets.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void encode(nlohmann::json& j, const RuleSets::Search::Parameters& p);
void encode(nlohmann::json& j, const RuleSets::EvaluateRules::Parameters& p);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULESETS_H_
