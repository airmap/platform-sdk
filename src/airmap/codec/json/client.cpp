//
//  client.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/client.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Client::Configuration& configuration) {
  get(configuration.host, j, "host");
  get(configuration.version, j, "version");
  get(configuration.sso.host, j["sso"], "host");
  get(configuration.sso.port, j["sso"], "port");
  get(configuration.telemetry.host, j["telemetry"], "host");
  get(configuration.telemetry.port, j["telemetry"], "port");
  get(configuration.traffic.host, j["traffic"], "host");
  get(configuration.traffic.port, j["traffic"], "port");
  get(configuration.credentials, j, "credentials");
}

void airmap::codec::json::decode(const nlohmann::json& j, Client::Version& version) {
  version = boost::lexical_cast<Client::Version>(j.get<std::string>());
}

void airmap::codec::json::encode(nlohmann::json& j, const Client::Configuration& configuration) {
  j["host"]              = configuration.host;
  j["version"]           = configuration.version;
  j["sso"]["host"]       = configuration.sso.host;
  j["sso"]["port"]       = configuration.sso.port;
  j["telemetry"]["host"] = configuration.telemetry.host;
  j["telemetry"]["port"] = configuration.telemetry.port;
  j["traffic"]["host"]   = configuration.traffic.host;
  j["traffic"]["port"]   = configuration.traffic.port;
  j["credentials"]       = configuration.credentials;
}

void airmap::codec::json::encode(nlohmann::json& j, Client::Version version) {
  j = boost::lexical_cast<std::string>(version);
}
