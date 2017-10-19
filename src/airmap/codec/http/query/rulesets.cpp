#include <airmap/codec/http/query/rulesets.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

#include <sstream>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const RuleSets::Search::Parameters& parameters) {
  if (parameters.geometry) {
    nlohmann::json geometry;
    geometry          = parameters.geometry.get();
    query["geometry"] = geometry.dump();
  }
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const RuleSets::FetchRules::Parameters& parameters) {
  if (parameters.rulesets)
    query["rulesets"] = parameters.rulesets.get();
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const RuleSets::Evaluation::Parameters& parameters) {
  if (parameters.rulesets)
    query["rulesets"] = parameters.rulesets.get();
  if (!parameters.features.empty()) {
    nlohmann::json features;
    for (const auto& pair : parameters.features) {
      features[pair.first] = pair.second;
    }
    query["flight_features"] = features.dump();
  }
  if (parameters.geometry) {
    nlohmann::json geometry;
    geometry          = parameters.geometry.get();
    query["geometry"] = geometry.dump();
  }
}
