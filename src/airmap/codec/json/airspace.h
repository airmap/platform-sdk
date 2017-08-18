
#ifndef AIRMAP_CODEC_JSON_AIRSPACE_H_
#define AIRMAP_CODEC_JSON_AIRSPACE_H_

#include <airmap/airspace.h>

#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/rule.h>

#include <string>
#include <vector>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Airspace& airspace) {
  airspace.set_id(j["id"].get<std::string>());

  if (j.count("name") > 0)
    airspace.set_name(j["name"].get<std::string>());
  if (j.count("country") > 0)
    airspace.set_country(j["country"].get<std::string>());
  if (j.count("state") > 0)
    airspace.set_state(j["state"].get<std::string>());
  if (j.count("city") > 0 && !j["city"].is_null())
    airspace.set_city(j["city"].get<std::string>());
  if (j.count("geometry") > 0)
    airspace.set_geometry(j["geometry"].get<Geometry>());
  if (j.count("related_geometry") > 0)
    airspace.set_related_geometries(j["related_geometry"].get<std::vector<Geometry>>());
  if (j.count("rules") > 0)
    airspace.set_rules(j["rules"].get<std::vector<Rule>>());
  if (j.count("last_updated") > 0)
    airspace.set_last_updated(iso8601::parse(j["last_updated"].get<std::string>()));
  if (j.count("type") > 0) {
    switch (j["type"].get<Airspace::Type>()) {
      case Airspace::Type::airport:
        airspace.set_details(j["properties"].get<Airport>());
        break;
      case Airspace::Type::controlled_airspace:
        airspace.set_details(j["properties"].get<ControlledAirspace>());
        break;
      case Airspace::Type::special_use_airspace:
        airspace.set_details(j["properties"].get<SpecialUseAirspace>());
        break;
      case Airspace::Type::tfr:
        airspace.set_details(j["properties"].get<TemporaryFlightRestriction>());
        break;
      case Airspace::Type::wildfire:
        airspace.set_details(j["properties"].get<Wildfire>());
        break;
      case Airspace::Type::park:
        airspace.set_details(j["properties"].get<Park>());
        break;
      case Airspace::Type::power_plant:
        airspace.set_details(j["properties"].get<PowerPlant>());
        break;
      case Airspace::Type::heliport:
        airspace.set_details(j["properties"].get<Heliport>());
        break;
      case Airspace::Type::prison:
        airspace.set_details(j["properties"].get<Prison>());
        break;
      case Airspace::Type::school:
        airspace.set_details(j["properties"].get<School>());
        break;
      case Airspace::Type::hospital:
        airspace.set_details(j["properties"].get<Hospital>());
        break;
      case Airspace::Type::fire:
        airspace.set_details(j["properties"].get<Fire>());
        break;
      case Airspace::Type::emergency:
        airspace.set_details(j["properties"].get<Emergency>());
        break;
      default:
        break;
    }
  }
}

inline void decode(const nlohmann::json& j, std::vector<Airspace>& v) {
  for (auto element : j) {
    v.push_back(Airspace{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Airspace::Type& type) {
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

inline void decode(const nlohmann::json& j, Airport& airport) {
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

inline void decode(const nlohmann::json& j, Airport::Runway& runway) {
  get(runway.name, j, "name");
  get(runway.length, j, "length");
  get(runway.bearing, j, "bearing");
}

inline void decode(const nlohmann::json& j, std::vector<Airport::Runway>& v) {
  for (auto element : j) {
    v.push_back(Airport::Runway{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json&, Airport::Use&) {
}
inline void decode(const nlohmann::json&, ControlledAirspace&) {
}
inline void decode(const nlohmann::json& j, SpecialUseAirspace& sua) {
  sua.type = j["type"].get<SpecialUseAirspace::Type>();
}
inline void decode(const nlohmann::json&, SpecialUseAirspace::Type&) {
}
inline void decode(const nlohmann::json& j, TemporaryFlightRestriction& tfr) {
  get(tfr.url, j, "url");
  get(tfr.type, j, "type");
  get(tfr.reason, j, "notam_reason");
}

inline void decode(const nlohmann::json&, TemporaryFlightRestriction::Type&) {
}

inline void decode(const nlohmann::json& j, Wildfire& wf) {
  get(wf.effective_date, j, "date_effective");
}

inline void decode(const nlohmann::json&, Park&) {
}
inline void decode(const nlohmann::json&, Prison&) {
}
inline void decode(const nlohmann::json&, School&) {
}
inline void decode(const nlohmann::json&, Hospital&) {
}
inline void decode(const nlohmann::json&, Fire&) {
}
inline void decode(const nlohmann::json&, Emergency&) {
}

inline void decode(const nlohmann::json& j, Heliport& hp) {
  get(hp.faa_id, j, "faa");
  get(hp.phone, j, "phone");
  get(hp.usage, j, "usage");
}

inline void decode(const nlohmann::json&, Heliport::Usage&) {
}

inline void decode(const nlohmann::json& j, PowerPlant& pp) {
  get(pp.technology, j, "tech");
  get(pp.code, j, "plant_code");
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AIRSPACE_H_
