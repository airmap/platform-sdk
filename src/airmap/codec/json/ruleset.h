
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
void decode(const nlohmann::json& j, RuleSet::Rule::Status& s);
void decode(const nlohmann::json& j, RuleSet::Rule::FlightFeature& f);
void decode(const nlohmann::json& j, std::vector<RuleSet::Rule::FlightFeature>& v);
void decode(const nlohmann::json& j, std::vector<RuleSet>& v);
void decode(const nlohmann::json& j, RuleSet::SelectionType& t);
void decode(const nlohmann::json& j, RuleSet::Jurisdiction& jd);
void decode(const nlohmann::json& j, RuleSet::Jurisdiction::Region& r);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULESET_H_
