//
//  flights.h
//  Airmap Platform SDK
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