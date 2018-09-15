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
//  credentials.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/credentials.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Credentials::Anonymous& anonymous) {
  get(anonymous.id, j, "id");
}

void airmap::codec::json::decode(const nlohmann::json& j, Credentials::OAuth& oauth) {
  get(oauth.username, j, "username");
  get(oauth.password, j, "password");
  get(oauth.client_id, j, "client-id");
  get(oauth.device_id, j, "device-id");
}

void airmap::codec::json::decode(const nlohmann::json& j, Credentials& credentials) {
  get(credentials.api_key, j, "api-key");
  get(credentials.anonymous, j, "anonymous");
  get(credentials.oauth, j, "oauth");
}

void airmap::codec::json::encode(nlohmann::json& j, const Credentials::OAuth& oauth) {
  j["username"]  = oauth.username;
  j["password"]  = oauth.password;
  j["client-id"] = oauth.client_id;
  j["device-id"] = oauth.device_id;
}

void airmap::codec::json::encode(nlohmann::json& j, const Credentials& credentials) {
  j["api-key"] = credentials.api_key;
  if (credentials.anonymous)
    j["anonymous"] = credentials.anonymous;
  if (credentials.oauth)
    j["oauth"] = credentials.oauth;
}
