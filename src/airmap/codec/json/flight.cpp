//
//  flight.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/flight.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>

void airmap::codec::json::decode(const nlohmann::json& j, Flight& f) {
  get(f.id, j, "id");
  get(f.latitude, j, "latitude");
  get(f.longitude, j, "longitude");
  get(f.max_altitude, j, "max_altitude");
  get(f.geometry, j, "geometry");
  get(f.created_at, j, "created_at");
  get(f.start_time, j, "start_time");
  get(f.end_time, j, "end_time");
  get(f.pilot, j, "pilot");
  get(f.pilot.id, j, "pilot_id");
  get(f.aircraft, j, "aircraft");
  get(f.aircraft.id, j, "aircraft_id");
  get(f.flight_plan_id, j, "flight_plan_id");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Flight>& f) {
  for (const auto& element : j) {
    f.emplace_back();
    f.back() = element;
  }
}
