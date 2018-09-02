//
//  monitor_mids.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_MONITOR_MIDS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_MONITOR_MIDS_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/net/mqtt/broker.h>
#include <airmap/net/mqtt/client.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

#include <nlohmann/json.hpp>

#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class MonitorMids : public util::cli::CommandWithFlagsAndAction {
 public:
  MonitorMids();

 private:
  using Mids         = util::TaggedString<util::tags::MustNotBeEmpty>;
  using MqttHost     = util::TaggedString<util::tags::MustNotBeEmpty>;
  using MqttUsername = util::TaggedString<util::tags::MustNotBeEmpty>;
  using MqttPassword = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<net::mqtt::Broker> broker_;
  std::shared_ptr<net::mqtt::Client> client_;
  std::set<std::unique_ptr<net::mqtt::Client::Subscription>> subscriptions_;
  struct {
    Logger::Severity log_level{Logger::Severity::info};
    Optional<Mids> mids;
    struct {
      Required<MqttHost> host;
      Required<std::uint16_t> port;
      Required<MqttUsername> username;
      Required<MqttPassword> password;
    } mqtt;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_MONITOR_MIDS_H_
