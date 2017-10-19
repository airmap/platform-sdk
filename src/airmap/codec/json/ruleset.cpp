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
  get(r.jurisdiction, j, "jurisdiction");
  get(r.airspace_types, j, "airspace_types");
  get(r.rules, j, "rules");
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSet& r) {
  j["id"]             = r.id;
  j["selection_type"] = r.selection_type;
  j["name"]           = r.name;
  j["short_name"]     = r.short_name;
  j["description"]    = r.description;
  j["default"]        = r.is_default;
  j["jurisdiction"]   = r.jurisdiction;

  for (const auto& airspace_type : r.airspace_types)
    j["airspace_types"].push_back(airspace_type);

  // for (const auto& rule : r.rules)
  //  j["rules"].push_back(rule);
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

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature& f) {
  get(f.id, j, "id");
  get(f.code, j, "code");
  get(f.name, j, "flight_feature");
  get(f.description, j, "description");
  get(f.status, j, "status");
  get(f.type, j, "input_type");
  get(f.measurement, j, "measurement_type");
  get(f.unit, j, "measurement_unit");
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Type& f) {
  f = boost::lexical_cast<RuleSet::Feature::Type>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Measurement& m) {
  m = boost::lexical_cast<RuleSet::Feature::Measurement>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Unit& u) {
  u = boost::lexical_cast<RuleSet::Feature::Unit>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Value& v) {
  switch (j.type()) {
    case nlohmann::json::value_t::boolean:
      v = RuleSet::Feature::Value{j.get<bool>()};
      break;
    case nlohmann::json::value_t::string:
      v = RuleSet::Feature::Value{j.get<std::string>()};
      break;
    case nlohmann::json::value_t::number_integer:
    case nlohmann::json::value_t::number_unsigned:
    case nlohmann::json::value_t::number_float:
      v = RuleSet::Feature::Value{j.get<double>()};
      break;
    default:
      v = RuleSet::Feature::Value{};
      break;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet::Feature>& v) {
  for (auto element : j) {
    v.push_back(RuleSet::Feature{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Rule::Status& s) {
  s = boost::lexical_cast<RuleSet::Rule::Status>(j.get<std::string>());
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

void airmap::codec::json::encode(nlohmann::json& j, const RuleSet::Feature::Value& v) {
  switch (v.type()) {
    case RuleSet::Feature::Type::boolean:
      j = v.boolean();
      break;
    case RuleSet::Feature::Type::floating_point:
      j = v.floating_point();
      break;
    case RuleSet::Feature::Type::string:
      j = v.string();
      break;
    default:
      break;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSet::SelectionType& t) {
  switch (t) {
    case RuleSet::SelectionType::optional:
      j["selection_type"] = "optional";
      break;
    case RuleSet::SelectionType::required:
      j["selection_type"] = "required";
      break;
    case RuleSet::SelectionType::pickone:
      j["selection_type"] = "pickone";
      break;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSet::Jurisdiction& jd) {
  j["id"]     = jd.id;
  j["name"]   = jd.name;
  j["region"] = jd.region;
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSet::Jurisdiction::Region& r) {
  switch (r) {
    case RuleSet::Jurisdiction::Region::national:
      j["region"] = "national";
      break;
    case RuleSet::Jurisdiction::Region::state:
      j["region"] = "state";
      break;
    case RuleSet::Jurisdiction::Region::county:
      j["region"] = "county";
      break;
    case RuleSet::Jurisdiction::Region::city:
      j["region"] = "city";
      break;
    case RuleSet::Jurisdiction::Region::local:
      j["region"] = "local";
      break;
  }
}
