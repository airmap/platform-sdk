// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <airmap/codec/json/flights.h>

#include <airmap/codec.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::encode(nlohmann::json& j, const Flights::CreateFlight::Parameters& parameters) {
  j["latitude"]     = parameters.latitude;
  j["longitude"]    = parameters.longitude;
  j["max_altitude"] = parameters.max_altitude;
  if (!parameters.aircraft_id.empty())
    j["aircraft_id"] = parameters.aircraft_id;
  j["start_time"] = parameters.start_time;
  j["end_time"]   = parameters.end_time;
  j["public"]     = parameters.is_public;
  j["notify"]     = parameters.give_digital_notice;
  j["buffer"]     = parameters.buffer;
  if (parameters.geometry) {
    j["geometry"] = parameters.geometry.get();
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Flights::Search::Response::Paging& paging) {
  get(paging.limit, j, "limit");
  get(paging.total, j, "total");
}

void airmap::codec::json::decode(const nlohmann::json& j, Flights::Search::Response& response) {
  get(response.paging, j, "paging");
  get(response.flights, j, "results");
}

void airmap::codec::json::decode(const nlohmann::json& j, Flights::EndFlight::Response& response) {
  get(response.end_time, j, "end_time");
}

void airmap::codec::json::decode(const nlohmann::json& j, Flights::DeleteFlight::Response& response) {
  get(response.id, j, "id");
}

void airmap::codec::json::decode(const nlohmann::json& j, Flights::StartFlightCommunications::Response& response) {
  get(response.key, j, "key");
}

void airmap::codec::json::decode(const nlohmann::json&, Flights::EndFlightCommunications::Response&) {
}
