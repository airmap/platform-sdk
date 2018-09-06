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
