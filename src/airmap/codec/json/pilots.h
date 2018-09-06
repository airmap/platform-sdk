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