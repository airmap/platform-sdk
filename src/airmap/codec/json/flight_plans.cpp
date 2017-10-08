#include <airmap/codec/json/flight_plan.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlans::Delete::Response& r) {
  get(r.id, j, "id");
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlans::Create::Parameters& p) {
  get(p.latitude, j, "takeoff_latitude");
  get(p.longitude, j, "takeoff_longitude");
  get(p.max_altitude, j, "max_altitude_agl");
  get(p.min_altitude, j, "min_altitude_agl");
  get(p.geometry, j, "geometry");
  get(p.start_time, j, "start_time");
  get(p.end_time, j, "end_time");
  get(p.pilot.id, j, "pilot_id");
  get(p.aircraft.id, j, "aircraft_id");
  get(p.buffer, j, "buffer");
  get(p.rulesets, j, "rulesets");
}

void airmap::codec::json::encode(nlohmann::json& j, const FlightPlans::Create::Parameters& p) {
  j["takeoff_latitude"]  = p.latitude;
  j["takeoff_longitude"] = p.longitude;
  j["max_altitude_agl"]  = p.max_altitude;
  j["min_altitude_agl"]  = p.min_altitude;
  j["geometry"]          = p.geometry;
  j["start_time"]        = p.start_time;
  j["end_time"]          = p.end_time;
  j["pilot_id"]          = p.pilot.id;
  j["aircraft_id"]       = p.aircraft.id;
  j["buffer"]            = p.buffer;

  for (const auto& id : p.rulesets)
    j["rulesets"].push_back(id);
}
