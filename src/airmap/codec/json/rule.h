//
//  rule.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_RULE_H_
#define AIRMAP_CODEC_JSON_RULE_H_

#include <airmap/rule.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Rule& rule);
void decode(const nlohmann::json& j, std::vector<Rule>& v);
void decode(const nlohmann::json&, Rule::Type&);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULE_H_