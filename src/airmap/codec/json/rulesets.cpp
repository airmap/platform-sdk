#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/rulesets.h>

void airmap::codec::json::encode(nlohmann::json& j, const RuleSets::Search::Parameters& p) {
  j["geometry"] = p.geometry;
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSets::EvaluateRules::Parameters& p) {
  j["geometry"] = p.geometry;
  if (!p.features.empty()) {
    nlohmann::json features;
    for (const auto& pair : p.features) {
      features[pair.first] = pair.second;
    }
    j["flight_features"] = features;
  }
  j["rulesets"] = p.rulesets;
}
