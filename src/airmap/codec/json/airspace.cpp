#include <airmap/codec/json/airspace.h>

#include <airmap/codec.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/rule.h>

void airmap::codec::json::decode(const nlohmann::json& j, Airspace& airspace) {
  airspace.set_id(j["id"].get<std::string>());

  if (j.count("name") > 0 && !j["name"].is_null())
    airspace.set_name(j["name"].get<std::string>());
  if (j.count("country") > 0 && !j["country"].is_null())
    airspace.set_country(j["country"].get<std::string>());
  if (j.count("state") > 0 && !j["state"].is_null())
    airspace.set_state(j["state"].get<std::string>());
  if (j.count("city") > 0 && !j["city"].is_null())
    airspace.set_city(j["city"].get<std::string>());
  if (j.count("geometry") > 0)
    airspace.set_geometry(j["geometry"].get<Geometry>());
  if (j.count("related_geometry") > 0)
    airspace.set_related_geometries(j["related_geometry"].get<std::map<std::string, Airspace::RelatedGeometry>>());
  if (j.count("rules") > 0)
    airspace.set_rules(j["rules"].get<std::vector<Rule>>());
  if (j.count("last_updated") > 0 && !j["last_updated"].is_null())
    airspace.set_last_updated(iso8601::parse(j["last_updated"].get<std::string>()));
  if (j.count("type") > 0) {
    switch (j["type"].get<Airspace::Type>()) {
      case Airspace::Type::airport:
        airspace.set_details(j["properties"].get<Airspace::Airport>());
        break;
      case Airspace::Type::controlled_airspace:
        airspace.set_details(j["properties"].get<Airspace::ControlledAirspace>());
        break;
      case Airspace::Type::special_use_airspace:
        airspace.set_details(j["properties"].get<Airspace::SpecialUseAirspace>());
        break;
      case Airspace::Type::tfr:
        airspace.set_details(j["properties"].get<Airspace::TemporaryFlightRestriction>());
        break;
      case Airspace::Type::wildfire:
        airspace.set_details(j["properties"].get<Airspace::Wildfire>());
        break;
      case Airspace::Type::park:
        airspace.set_details(j["properties"].get<Airspace::Park>());
        break;
      case Airspace::Type::power_plant:
        airspace.set_details(j["properties"].get<Airspace::PowerPlant>());
        break;
      case Airspace::Type::heliport:
        airspace.set_details(j["properties"].get<Airspace::Heliport>());
        break;
      case Airspace::Type::prison:
        airspace.set_details(j["properties"].get<Airspace::Prison>());
        break;
      case Airspace::Type::school:
        airspace.set_details(j["properties"].get<Airspace::School>());
        break;
      case Airspace::Type::hospital:
        airspace.set_details(j["properties"].get<Airspace::Hospital>());
        break;
      case Airspace::Type::fire:
        airspace.set_details(j["properties"].get<Airspace::Fire>());
        break;
      case Airspace::Type::emergency:
        airspace.set_details(j["properties"].get<Airspace::Emergency>());
        break;
      default:
        break;
    }
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Airspace>& v) {
  for (auto element : j) {
    v.push_back(Airspace{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Airspace::RelatedGeometry& rg) {
  get(rg.id, j, "id");
  get(rg.geometry, j, "geometry");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::map<std::string, Airspace::RelatedGeometry>& rg) {
  for (auto it   = j.begin(); it != j.end(); ++it)
    rg[it.key()] = it.value();
}

void airmap::codec::json::decode(const nlohmann::json& j, Airspace::Type& type) {
  auto t = j.get<std::string>();

  if (t == "airport")
    type = Airspace::Type::airport;
  else if (t == "controlled_airspace")
    type = Airspace::Type::controlled_airspace;
  else if (t == "special_use_airspace")
    type = Airspace::Type::special_use_airspace;
  else if (t == "tfr")
    type = Airspace::Type::tfr;
  else if (t == "wildfire")
    type = Airspace::Type::wildfire;
  else if (t == "park")
    type = Airspace::Type::park;
  else if (t == "power_plant")
    type = Airspace::Type::power_plant;
  else if (t == "heliport")
    type = Airspace::Type::heliport;
  else if (t == "prison")
    type = Airspace::Type::prison;
  else if (t == "school")
    type = Airspace::Type::school;
  else if (t == "hospital")
    type = Airspace::Type::hospital;
  else if (t == "fire")
    type = Airspace::Type::fire;
  else if (t == "wildfire")
    type = Airspace::Type::wildfire;
}

void airmap::codec::json::decode(const nlohmann::json& j, Airspace::Airport& airport) {
  get(airport.iata, j, "iata");
  get(airport.icao, j, "icao");
  get(airport.paved, j, "paved");
  get(airport.phone, j, "phone");
  get(airport.tower, j, "tower");
  get(airport.runways, j, "runways");
  get(airport.elevation, j, "elevation");
  get(airport.longest_runway, j, "longest_runway");
  get(airport.use, j, "use");
}

void airmap::codec::json::decode(const nlohmann::json& j, Airspace::Airport::Runway& runway) {
  get(runway.name, j, "name");
  get(runway.length, j, "length");
  get(runway.bearing, j, "bearing");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Airspace::Airport::Runway>& v) {
  for (auto element : j) {
    v.push_back(Airspace::Airport::Runway{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json&, Airspace::Airport::Use&) {
}
void airmap::codec::json::decode(const nlohmann::json&, Airspace::ControlledAirspace&) {
}
void airmap::codec::json::decode(const nlohmann::json& j, Airspace::SpecialUseAirspace& sua) {
  sua.type = j["type"].get<Airspace::SpecialUseAirspace::Type>();
}
void airmap::codec::json::decode(const nlohmann::json&, Airspace::SpecialUseAirspace::Type&) {
}
void airmap::codec::json::decode(const nlohmann::json& j, Airspace::TemporaryFlightRestriction& tfr) {
  get(tfr.url, j, "url");
  get(tfr.type, j, "type");
  get(tfr.reason, j, "notam_reason");
}

void airmap::codec::json::decode(const nlohmann::json&, Airspace::TemporaryFlightRestriction::Type&) {
}

void airmap::codec::json::decode(const nlohmann::json& j, Airspace::Wildfire& wf) {
  get(wf.effective_date, j, "date_effective");
}

void airmap::codec::json::decode(const nlohmann::json&, Airspace::Park&) {
}
void airmap::codec::json::decode(const nlohmann::json&, Airspace::Prison&) {
}
void airmap::codec::json::decode(const nlohmann::json&, Airspace::School&) {
}
void airmap::codec::json::decode(const nlohmann::json&, Airspace::Hospital&) {
}
void airmap::codec::json::decode(const nlohmann::json&, Airspace::Fire&) {
}
void airmap::codec::json::decode(const nlohmann::json&, Airspace::Emergency&) {
}

void airmap::codec::json::decode(const nlohmann::json& j, Airspace::Heliport& hp) {
  get(hp.faa_id, j, "faa");
  get(hp.phone, j, "phone");
  get(hp.usage, j, "usage");
}

void airmap::codec::json::decode(const nlohmann::json&, Airspace::Heliport::Usage&) {
}

void airmap::codec::json::decode(const nlohmann::json& j, Airspace::PowerPlant& pp) {
  get(pp.technology, j, "tech");
  get(pp.code, j, "plant_code");
}
