
#ifndef AIRMAP_CODEC_JSON_FLIGHT_PLAN_H_
#define AIRMAP_CODEC_JSON_FLIGHT_PLAN_H_

#include <airmap/flight_plan.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, FlightPlan& p);
void encode(nlohmann::json& j, const FlightPlan& p);

void decode(const nlohmann::json& j, FlightPlan::Briefing& b);
void decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet& r);
void decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet::Rule& r);
void decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::RuleSet::Rule>& v);
void decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet::Rule::Status& s);
void decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::RuleSet>& v);
void decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet::Type& t);
void decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::RuleSet::Id>& v);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Jurisdiction& jd);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Jurisdiction::Region& r);
void decode(const nlohmann::json& j, FlightPlan::Briefing::AdvisoryStatus& a);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Authority& a);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Authorization& a);
void decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::Authorization>& v);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Authorization::Status& s);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Validation& v);
void decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::Validation>& v);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Validation::Feature& f);
void decode(const nlohmann::json& j, FlightPlan::Briefing::Validation::Status& s);
void encode(nlohmann::json& j, const std::vector<FlightPlan::Briefing::RuleSet::Id>& v);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHT_PLAN_H_
