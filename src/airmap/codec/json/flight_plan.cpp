#include <airmap/codec/json/flight_plan.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlan& p) {
  get(p.id, j, "id");
  get(p.latitude, j, "latitude");
  get(p.longitude, j, "longitude");
  get(p.max_altitude, j, "max_altitude");
  get(p.min_altitude, j, "min_altitude");
  get(p.geometry, j, "geometry");
  get(p.start_time, j, "start_time");
  get(p.end_time, j, "end_time");
  get(p.pilot.id, j, "pilot_id");
  get(p.aircraft.id, j, "aircraft_id");
  get(p.buffer, j, "buffer");
}

void airmap::codec::json::encode(nlohmann::json& j, const FlightPlan& p) {
  if (p.id)
    j["id"] = p.id;
  j["latitude"] = p.latitude;
  j["longitude"] = p.longitude;
  j["max_altitude"] = p.max_altitude;
  j["min_altitude"] = p.min_altitude;
  j["geometry"] = p.geometry;
  j["start_time"] = p.start_time;
  j["end_time"] = p.end_time;
  j["pilot_id"] = p.pilot.id;
  j["aircraft_id"] = p.aircraft.id;
  j["buffer"] = p.buffer;
}
