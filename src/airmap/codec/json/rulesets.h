//
//  rulesets.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_RULESETS_H_
#define AIRMAP_CODEC_JSON_RULESETS_H_

#include <airmap/rulesets.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, RuleSets::EvaluateRules::Parameters& p);
void encode(nlohmann::json& j, const RuleSets::EvaluateRules::Parameters& p);
void encode(nlohmann::json& j, const RuleSets::Search::Parameters& p);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULESETS_H_
