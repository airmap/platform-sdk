/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  ruleset.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_RULESET_H_
#define AIRMAP_CODEC_JSON_RULESET_H_

#include <airmap/ruleset.h>

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, RuleSet& r);
void decode(const nlohmann::json& j, RuleSet::Rule& r);
void decode(const nlohmann::json& j, std::vector<RuleSet::Rule>& v);
void decode(const nlohmann::json& j, RuleSet::Feature& f);
void decode(const nlohmann::json& j, std::vector<RuleSet::Feature>& v);
void decode(const nlohmann::json& j, RuleSet::Feature::Type& f);
void decode(const nlohmann::json& j, RuleSet::Feature::Measurement& m);
void decode(const nlohmann::json& j, RuleSet::Feature::Unit& u);
void decode(const nlohmann::json& j, RuleSet::Feature::Value& v);
void decode(const nlohmann::json& j, RuleSet::Rule::Status& s);
void decode(const nlohmann::json& j, std::vector<RuleSet>& v);
void decode(const nlohmann::json& j, RuleSet::SelectionType& t);
void decode(const nlohmann::json& j, RuleSet::Jurisdiction& jd);
void decode(const nlohmann::json& j, RuleSet::Jurisdiction::Region& r);
void encode(nlohmann::json& j, const RuleSet::Feature::Value& v);
void encode(nlohmann::json& j, const std::vector<RuleSet::Id>& v);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULESET_H_
