#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/rulesets.h>

void airmap::codec::json::decode(const nlohmann::json& j, RuleSets::EvaluateRules::Parameters& p) {
  get(p.geometry, j, "geometry");
  get(p.rulesets, j, "rulesets");
  if (j.count("flight_features") > 0) {
    auto ff = j["flight_features"];

    for (auto it = ff.begin(); it != ff.end(); ++it) {
      RuleSet::Feature::Value value = it.value();
      p.features.emplace(it.key(), value);
    }
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSets::EvaluateRules::Parameters& p) {
  j["geometry"] = p.geometry;
  j["rulesets"] = p.rulesets;

  for (const auto& pair : p.features) {
    j["flight_features"][pair.first] = pair.second;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSets::Search::Parameters& p) {
  j["geometry"] = p.geometry;
}
