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
