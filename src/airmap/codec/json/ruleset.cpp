#include <airmap/codec/json/ruleset.h>

#include <airmap/codec.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet& ruleset) {
  // ruleset.result = j.dump();
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet>& v) {
  for (auto element : j) {
    v.push_back(RuleSet{});
    v.back() = element;
  }
}