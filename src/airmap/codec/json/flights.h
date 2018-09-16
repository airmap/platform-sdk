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
//  flights.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_FLIGHTS_H_
#define AIRMAP_CODEC_JSON_FLIGHTS_H_

#include <airmap/flights.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace codec {
namespace json {

void encode(nlohmann::json& j, const Flights::CreateFlight::Parameters& parameters);
void decode(const nlohmann::json& j, Flights::Search::Response& response);
void decode(const nlohmann::json& j, Flights::Search::Response::Paging& paging);
void decode(const nlohmann::json& j, Flights::EndFlight::Response& response);
void decode(const nlohmann::json& j, Flights::DeleteFlight::Response& response);
void decode(const nlohmann::json& j, Flights::StartFlightCommunications::Response& response);
void decode(const nlohmann::json&, Flights::EndFlightCommunications::Response&);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHTS_H_