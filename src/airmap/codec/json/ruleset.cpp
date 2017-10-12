#include <airmap/codec/json/ruleset.h>

#include <airmap/codec.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::Jurisdiction::Region region) {
  switch (region) {
    case RuleSet::Jurisdiction::Region::national:
      return out << "national";
    case RuleSet::Jurisdiction::Region::state:
      return out << "state";
    case RuleSet::Jurisdiction::Region::county:
      return out << "county";
    case RuleSet::Jurisdiction::Region::city:
      return out << "city";
    case RuleSet::Jurisdiction::Region::local:
      return out << "local";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::Jurisdiction::Region& region) {
  std::string s;
  in >> s;

  if (s == "national" || s == "federal") {
    region = RuleSet::Jurisdiction::Region::national;
  } else if (s == "state") {
    region = RuleSet::Jurisdiction::Region::state;
  } else if (s == "county") {
    region = RuleSet::Jurisdiction::Region::county;
  } else if (s == "city") {
    region = RuleSet::Jurisdiction::Region::city;
  } else if (s == "local") {
    region = RuleSet::Jurisdiction::Region::local;
  }
  
  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::SelectionType type) {
  switch (type) {
    case RuleSet::SelectionType::optional:
      return out << "optional";
    case RuleSet::SelectionType::required:
      return out << "required";
    case RuleSet::SelectionType::pickone:
      return out << "pickone";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::SelectionType& type) {
  std::string s;
  in >> s;

  if (s == "optional") {
    type = RuleSet::SelectionType::optional;
  } else if (s == "required") {
    type = RuleSet::SelectionType::required;
  } else if (s == "pickone") {
    type = RuleSet::SelectionType::pickone;
  } else if (s == "pick1") {
    type = RuleSet::SelectionType::pickone;
  }

  return in;

}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::Rule::Status status) {
  switch (status) {
    case RuleSet::Rule::Status::unknown:
      return out << "unknown";
    case RuleSet::Rule::Status::conflicting:
      return out << "conflicting";
    case RuleSet::Rule::Status::not_conflicting:
      return out << "not_conflicting";
    case RuleSet::Rule::Status::missing_info:
      return out << "missing_info";
    case RuleSet::Rule::Status::informational:
      return out << "informational";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::Rule::Status& status) {
  std::string s;
  in >> s;

  if (s == "conflicting") {
    status = RuleSet::Rule::Status::conflicting;
  } else if (s == "not_conflicting") {
    status = RuleSet::Rule::Status::not_conflicting;
  } else if (s == "missing_info") {
    status = RuleSet::Rule::Status::missing_info;
  } else if (s == "informational" || s == "informational_rules") {
    status = RuleSet::Rule::Status::informational;
  } else {
    status = RuleSet::Rule::Status::unknown;
  }

  return in;
}



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
