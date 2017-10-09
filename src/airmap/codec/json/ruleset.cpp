#include <airmap/codec/json/ruleset.h>

#include <airmap/codec.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet& r) {
  get(r.id, j, "id");
  get(r.selection_type, j, "selection_type");
  get(r.name, j, "name");
  get(r.short_name, j, "short_name");
  get(r.description, j, "description");
  get(r.is_default, j, "default");
  get(r.airspace_types, j, "airspace_types");
  get(r.rules, j, "rules");
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Rule& r) {
  get(r.short_text, j, "short_text");
  get(r.description, j, "description");
  get(r.status, j, "status");
  get(r.display_order, j, "display_order");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet::Rule>& v) {
  for (auto element : j) {
    v.push_back(RuleSet::Rule{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Rule::Status& s) {
  s = boost::lexical_cast<RuleSet::Rule::Status>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Rule::FlightFeature& f) {
  get(f.flight_feature, j, "flight_feature");
  get(f.description, j, "description");
  get(f.input_type, j, "input_type");
  get(f.measurement_type, j, "measurement_type");
  get(f.measurement_unit, j, "measurement_unit");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet::Rule::FlightFeature>& v) {
  for (auto element : j) {
    v.push_back(RuleSet::Rule::FlightFeature{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet>& v) {
  for (auto element : j) {
    v.push_back(RuleSet{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::SelectionType& t) {
  t = boost::lexical_cast<RuleSet::SelectionType>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Jurisdiction& jd) {
  get(jd.id, j, "id");
  get(jd.name, j, "name");
  get(jd.region, j, "region");
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Jurisdiction::Region& r) {
  r = boost::lexical_cast<RuleSet::Jurisdiction::Region>(j.get<std::string>());
}
