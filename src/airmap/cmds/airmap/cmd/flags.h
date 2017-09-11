#ifndef AIRMAP_CMDS_AIRMAP_CMD_FLAGS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_FLAGS_H_

#include <airmap/client.h>
#include <airmap/logger.h>

#include <airmap/util/cli.h>
#include <airmap/util/tagged_string.h>

#include <memory>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

using ApiKey        = util::TaggedString<util::tags::MustNotBeEmpty>;
using Authorization = util::TaggedString<util::tags::MustNotBeEmpty>;
using ClientId      = util::TaggedString<util::tags::MustNotBeEmpty>;
using ConfigFile    = util::TaggedString<util::tags::MustNotBeEmpty>;
using EncryptionKey = util::TaggedString<util::tags::MustNotBeEmpty>;
using FlightId      = util::TaggedString<util::tags::MustNotBeEmpty>;
using TelemetryHost = util::TaggedString<util::tags::MustNotBeEmpty>;
using UserId        = util::TaggedString<util::tags::MustNotBeEmpty>;

namespace flags {
std::shared_ptr<util::cli::Flag> api_key(Optional<ApiKey>& api_key);
std::shared_ptr<util::cli::Flag> authorization(Optional<Authorization>& authorization);
std::shared_ptr<util::cli::Flag> client_id(Optional<ClientId>& client_id);
std::shared_ptr<util::cli::Flag> config_file(Optional<ConfigFile>& config_file);
std::shared_ptr<util::cli::Flag> encryption_key(Optional<EncryptionKey>& key);
std::shared_ptr<util::cli::Flag> flight_id(Optional<FlightId>& flight_id);
std::shared_ptr<util::cli::Flag> log_level(Logger::Severity& severity);
std::shared_ptr<util::cli::Flag> telemetry_host(Optional<TelemetryHost>& host);
std::shared_ptr<util::cli::Flag> telemetry_port(Optional<std::uint16_t>& port);
std::shared_ptr<util::cli::Flag> user_id(Optional<UserId>& user_id);
std::shared_ptr<util::cli::Flag> version(Client::Version& version);
}  // namespace flags
}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_FLAGS_H_
