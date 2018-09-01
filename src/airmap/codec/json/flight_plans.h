//
//  flight_plans.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//


#ifndef AIRMAP_CODEC_JSON_FLIGHT_PLANS_H_
#define AIRMAP_CODEC_JSON_FLIGHT_PLANS_H_

#include <airmap/flight_plans.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, FlightPlans::Delete::Response& r);
void decode(const nlohmann::json& j, FlightPlans::Create::Parameters& p);
void encode(nlohmann::json& j, const FlightPlans::Create::Parameters& p);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHT_PLANS_H_
