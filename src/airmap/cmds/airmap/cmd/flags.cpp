//
//  flags.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/flags.h>

namespace cli   = airmap::util::cli;
namespace flags = airmap::cmds::airmap::cmd::flags;

std::shared_ptr<cli::Flag> flags::api_key(Optional<ApiKey>& api_key) {
  return cli::make_flag("api-key", "api-key for authenticating with the AirMap services", api_key);
}

std::shared_ptr<cli::Flag> flags::authorization(Optional<Authorization>& authorization) {
  return cli::make_flag("authorization", "token used for authorizing with the AirMap services", authorization);
}

std::shared_ptr<cli::Flag> flags::client_id(Optional<ClientId>& client_id) {
  return cli::make_flag("client-id", "id of client", client_id);
}

std::shared_ptr<cli::Flag> flags::config_file(Optional<ConfigFile>& config_file) {
  return cli::make_flag("config-file", "airmap configuration file", config_file);
}

std::shared_ptr<cli::Flag> flags::encryption_key(Optional<EncryptionKey>& key) {
  return cli::make_flag("encryption-key", "encryption key for telemetry encryption", key);
}

std::shared_ptr<cli::Flag> flags::flight_id(Optional<FlightId>& flight_id) {
  return cli::make_flag("flight-id", "id of flight", flight_id);
}

std::shared_ptr<cli::Flag> flags::log_level(Logger::Severity& severity) {
  return cli::make_flag("log-level", "log message with severity >= log-level", severity);
}

std::shared_ptr<cli::Flag> flags::telemetry_host(Optional<TelemetryHost>& host) {
  return cli::make_flag("telemetry-host", "telemetry host address", host);
}

std::shared_ptr<cli::Flag> flags::telemetry_port(Optional<std::uint16_t>& port) {
  return cli::make_flag("telemetry-port", "telemetry host port", port);
}

std::shared_ptr<cli::Flag> flags::token_file(Optional<TokenFile>& token_file) {
  return cli::make_flag("token-file", "airmap token file", token_file);
}

std::shared_ptr<cli::Flag> flags::user_id(Optional<UserId>& user_id) {
  return cli::make_flag("user-id", "user-id for authenticating with the AirMap services", user_id);
}

std::shared_ptr<cli::Flag> flags::version(Client::Version& version) {
  return cli::make_flag("version", "work against this version of the AirMap services", version);
}
