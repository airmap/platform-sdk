/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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
