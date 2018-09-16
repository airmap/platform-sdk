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
//  authenticator.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_JSON_AUTHENTICATOR_H_
#define AIRMAP_CODEC_JSON_AUTHENTICATOR_H_

#include <airmap/authenticator.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void encode(nlohmann::json& j, const Authenticator::GrantType& type);
void encode(nlohmann::json& j, const Authenticator::Scope& scope);
void encode(nlohmann::json& j, const Authenticator::Connection& connection);
void encode(nlohmann::json& j, const Authenticator::AuthenticateWithPassword::Params& params);
void encode(nlohmann::json& j, const Authenticator::AuthenticateAnonymously::Params& params);
void encode(nlohmann::json& j, const Authenticator::RenewAuthentication::Params& params);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AUTHENTICATOR_H_
