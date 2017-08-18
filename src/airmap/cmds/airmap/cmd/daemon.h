#ifndef AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_
#define AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_

#include <airmap/client.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Daemon : public util::cli::CommandWithFlagsAndAction {
 public:
  Daemon();

 private:
  using ApiKey        = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Host          = util::TaggedString<util::tags::MustNotBeEmpty>;
  using UserId        = util::TaggedString<util::tags::MustNotBeEmpty>;
  using AircraftId    = util::TaggedString<util::tags::MustNotBeEmpty>;
  using SerialDevice  = util::TaggedString<util::tags::MustNotBeEmpty>;
  using TcpEndpointIp = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_;

  Optional<ApiKey> api_key_;
  Optional<UserId> user_id_;
  Optional<AircraftId> aircraft_id_;
  Optional<SerialDevice> serial_device_;
  Optional<Host> telemetry_host_;
  Optional<std::uint16_t> telemetry_port_;
  Optional<TcpEndpointIp> tcp_endpoint_ip_;
  Optional<std::uint16_t> tcp_endpoint_port_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_
