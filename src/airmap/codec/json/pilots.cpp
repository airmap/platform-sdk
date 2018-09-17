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
#include <airmap/codec/json/pilots.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::UpdateForId::Parameters& parameters) {
  j["first_name"] = parameters.first_name;
  j["last_name"]  = parameters.last_name;
  j["username"]   = parameters.user_name;
  j["phone"]      = parameters.phone;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::FinishVerifyPilotPhoneForId::Parameters& parameters) {
  j["token"] = parameters.token;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::AddAircraft::Parameters& parameters) {
  j["model_id"] = parameters.model_id;
  j["nickname"] = parameters.nick_name;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::DeleteAircraft::Parameters& parameters) {
  j["aircraft_id"] = parameters.aircraft_id;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::UpdateAircraft::Parameters& parameters) {
  j["nick_name"] = parameters.nick_name;
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::StartVerifyPilotPhoneForId::Empty&) {
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::FinishVerifyPilotPhoneForId::Empty&) {
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::DeleteAircraft::Empty&) {
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::UpdateAircraft::Empty&) {
}
