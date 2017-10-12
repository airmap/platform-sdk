#include <airmap/codec/json/flight_plan.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan& p) {
  get(p.id, j, "id");
  get(p.takeoff.latitude, j, "takeoff_latitude");
  get(p.takeoff.longitude, j, "takeoff_longitude");
  get(p.altitude_agl.max, j, "max_altitude_agl");
  get(p.altitude_agl.min, j, "min_altitude_agl");
  get(p.geometry, j, "geometry");
  get(p.start_time, j, "start_time");
  get(p.end_time, j, "end_time");
  get(p.pilot.id, j, "pilot_id");
  get(p.aircraft.id, j, "aircraft_id");
  get(p.buffer, j, "buffer");
}

void airmap::codec::json::encode(nlohmann::json& j, const FlightPlan& p) {
  j["id"]                = p.id;
  j["takeoff_latitude"]  = p.takeoff.latitude;
  j["takeoff_longitude"] = p.takeoff.longitude;
  j["max_altitude_agl"]  = p.altitude_agl.max;
  j["min_altitude_agl"]  = p.altitude_agl.min;
  j["geometry"]          = p.geometry;
  j["start_time"]        = p.start_time;
  j["end_time"]          = p.end_time;
  j["pilot_id"]          = p.pilot.id;
  j["aircraft_id"]       = p.aircraft.id;
  j["buffer"]            = p.buffer;
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing& b) {
  get(b.created_at, j, "created_at");
  get(b.airspace, j, "airspace");
  get(b.rulesets, j, "rulesets");
  get(b.validations, j, "validations");
  get(b.authorizations, j, "authorizations");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Feature& f) {
  get(f.id, j, "id");
  get(f.code, j, "code");
  get(f.name, j, "flight_feature");
  get(f.description, j, "description");
  get(f.status, j, "status");
  get(f.type, j, "input_type");
  get(f.measurement, j, "measurement_type");
  get(f.unit, j, "measurement_unit");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Feature::Type& f) {
  f = boost::lexical_cast<FlightPlan::Briefing::Feature::Type>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Feature::Measurement& m) {
  m = boost::lexical_cast<FlightPlan::Briefing::Feature::Measurement>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Feature::Unit& u) {
  u = boost::lexical_cast<FlightPlan::Briefing::Feature::Unit>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Feature::Value& v) {
  switch (j.type()) {
    case nlohmann::json::value_t::boolean:
      v = FlightPlan::Briefing::Feature::Value{j.get<bool>()};
      break;
    case nlohmann::json::value_t::string:
      v = FlightPlan::Briefing::Feature::Value{j.get<std::string>()};
      break;
    case nlohmann::json::value_t::number_integer:
    case nlohmann::json::value_t::number_unsigned:
    case nlohmann::json::value_t::number_float:
      v = FlightPlan::Briefing::Feature::Value{j.get<double>()};
      break;
    default:
      v = FlightPlan::Briefing::Feature::Value{};
      break;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::Feature>& v) {
  for (auto element : j) {
    v.push_back(FlightPlan::Briefing::Feature{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet& r) {
  get(r.id, j, "id");
  get(r.type, j, "type");
  get(r.name, j, "name");
  get(r.short_name, j, "short_name");
  get(r.summary, j, "summary");
  get(r.is_default, j, "default");
  get(r.layers, j, "layers");
  get(r.rules, j, "rules");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet::Rule& r) {
  get(r.short_text, j, "short_text");
  get(r.description, j, "description");
  get(r.status, j, "status");
  get(r.display_order, j, "display_order");
  get(r.features, j, "flight_features");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::RuleSet::Rule>& v) {
  for (auto element : j) {
    v.push_back(FlightPlan::Briefing::RuleSet::Rule{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet::Rule::Status& s) {
  s = boost::lexical_cast<FlightPlan::Briefing::RuleSet::Rule::Status>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::RuleSet>& v) {
  for (auto element : j) {
    v.push_back(FlightPlan::Briefing::RuleSet{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::RuleSet::Id>& v) {
  for (auto element : j) {
    v.push_back(FlightPlan::Briefing::RuleSet::Id{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::RuleSet::Type& t) {
  t = boost::lexical_cast<FlightPlan::Briefing::RuleSet::Type>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Jurisdiction& jd) {
  get(jd.id, j, "id");
  get(jd.name, j, "name");
  get(jd.region, j, "region");
  get(jd.rulesets, j, "rulesets");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Jurisdiction::Region& r) {
  r = boost::lexical_cast<FlightPlan::Briefing::Jurisdiction::Region>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::AdvisoryStatus& a) {
  get(a.color, j, "color");
  get(a.advisories, j, "advisories");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Authority& a) {
  get(a.name, j, "name");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Authorization& a) {
  get(a.status, j, "status");
  get(a.authority, j, "authority");
  get(a.message, j, "message");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::Authorization>& v) {
  for (auto element : j) {
    v.push_back(FlightPlan::Briefing::Authorization{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Authorization::Status& s) {
  s = boost::lexical_cast<FlightPlan::Briefing::Authorization::Status>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Validation& v) {
  get(v.status, j, "status");
  get(v.data, j, "data");
  get(v.message, j, "message");
  get(v.feature, j, "feature");
  get(v.authority, j, "authority");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<FlightPlan::Briefing::Validation>& v) {
  for (auto element : j) {
    v.push_back(FlightPlan::Briefing::Validation{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Validation::Feature& f) {
  get(f.code, j, "code");
  get(f.description, j, "description");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::Validation::Status& s) {
  s = boost::lexical_cast<FlightPlan::Briefing::Validation::Status>(j.get<std::string>());
}

void airmap::codec::json::encode(nlohmann::json& j, const std::vector<FlightPlan::Briefing::RuleSet::Id>& v) {
  for (const auto& id : v) {
    j.push_back(id);
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const FlightPlan::Briefing::Feature::Value& v) {
  switch (v.type()) {
    case FlightPlan::Briefing::Feature::Type::boolean:
      j = v.boolean();
      break;
    case FlightPlan::Briefing::Feature::Type::floating_point:
      j = v.floating_point();
      break;
    case FlightPlan::Briefing::Feature::Type::string:
      j = v.string();
      break;
    default:
      break;
  }
}
