#include <airmap/codec/json/flight_plan.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/evaluation.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>
#include <airmap/codec/json/ruleset.h>

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan& p) {
  get(p.id, j, "id");
  get(p.flight_id, j, "flight_id");
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
  j["id"] = p.id;
  if (p.flight_id)
    j["flight_id"] = p.flight_id.get();
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
  decode(j, b.evaluation);
}

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan::Briefing::AdvisoryStatus& a) {
  get(a.color, j, "color");
  get(a.advisories, j, "advisories");
}
