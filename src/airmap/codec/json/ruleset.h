
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


}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULESET_H_
