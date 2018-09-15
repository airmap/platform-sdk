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
//  pilots.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_PILOTS_H_
#define AIRMAP_CODEC_JSON_PILOTS_H_

#include <airmap/pilots.h>

#include <airmap/codec/json/date_time.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace codec {
namespace json {

void encode(nlohmann::json& j, const Pilots::UpdateForId::Parameters& parameters);
void encode(nlohmann::json& j, const Pilots::FinishVerifyPilotPhoneForId::Parameters& parameters);
void encode(nlohmann::json& j, const Pilots::AddAircraft::Parameters& parameters);
void encode(nlohmann::json& j, const Pilots::DeleteAircraft::Parameters& parameters);
void encode(nlohmann::json& j, const Pilots::UpdateAircraft::Parameters& parameters);
void decode(const nlohmann::json&, Pilots::StartVerifyPilotPhoneForId::Empty&);
void decode(const nlohmann::json&, Pilots::FinishVerifyPilotPhoneForId::Empty&);
void decode(const nlohmann::json&, Pilots::DeleteAircraft::Empty&);
void decode(const nlohmann::json&, Pilots::UpdateAircraft::Empty&);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_PILOTS_H_