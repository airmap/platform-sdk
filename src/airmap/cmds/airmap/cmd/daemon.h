#ifndef AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_
#define AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_

#include <airmap/cmds/airmap/cmd/flags.h>

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
  using AircraftId    = util::TaggedString<util::tags::MustNotBeEmpty>;
  using SerialDevice  = util::TaggedString<util::tags::MustNotBeEmpty>;
  using TcpEndpointIp = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Required<ConfigFile> config_file_;
  Required<AircraftId> aircraft_id_;
  Required<SerialDevice> serial_device_;
  Required<TelemetryHost> telemetry_host_;
  Required<std::uint16_t> telemetry_port_;
  Required<TcpEndpointIp> tcp_endpoint_ip_;
  Required<std::uint16_t> tcp_endpoint_port_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_
