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
//  airspace.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_AIRSPACE_H_
#define AIRMAP_CODEC_JSON_AIRSPACE_H_

#include <airmap/airspace.h>

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Airspace& airspace);
void decode(const nlohmann::json& j, std::vector<Airspace>& v);
void decode(const nlohmann::json& j, Airspace::RelatedGeometry& rg);
void decode(const nlohmann::json& j, std::map<std::string, Airspace::RelatedGeometry>& rg);
void decode(const nlohmann::json& j, Airspace::Type& type);
void decode(const nlohmann::json& j, Airspace::Airport& airport);
void decode(const nlohmann::json& j, Airspace::Airport::Runway& runway);
void decode(const nlohmann::json& j, std::vector<Airspace::Airport::Runway>& v);
void decode(const nlohmann::json&, Airspace::Airport::Use&);
void decode(const nlohmann::json&, Airspace::ControlledAirspace&);
void decode(const nlohmann::json& j, Airspace::SpecialUseAirspace& sua);
void decode(const nlohmann::json&, Airspace::SpecialUseAirspace::Type&);
void decode(const nlohmann::json& j, Airspace::TemporaryFlightRestriction& tfr);
void decode(const nlohmann::json&, Airspace::TemporaryFlightRestriction::Type&);
void decode(const nlohmann::json& j, Airspace::Wildfire& wf);
void decode(const nlohmann::json&, Airspace::Park&);
void decode(const nlohmann::json&, Airspace::Prison&);
void decode(const nlohmann::json&, Airspace::School&);
void decode(const nlohmann::json&, Airspace::Hospital&);
void decode(const nlohmann::json&, Airspace::Fire&);
void decode(const nlohmann::json&, Airspace::Emergency&);
void decode(const nlohmann::json& j, Airspace::Heliport& hp);
void decode(const nlohmann::json&, Airspace::Heliport::Usage&);
void decode(const nlohmann::json& j, Airspace::PowerPlant& pp);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AIRSPACE_H_
