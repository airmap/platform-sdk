//
//  flight_plan.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//


#ifndef AIRMAP_CODEC_JSON_FLIGHT_PLAN_H_
#define AIRMAP_CODEC_JSON_FLIGHT_PLAN_H_

#include <airmap/flight_plan.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, FlightPlan& p);
void encode(nlohmann::json& j, const FlightPlan& p);

void decode(const nlohmann::json& j, FlightPlan::Briefing& b);
void decode(const nlohmann::json& j, FlightPlan::Briefing::AdvisoryStatus& a);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHT_PLAN_H_
