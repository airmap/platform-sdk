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
