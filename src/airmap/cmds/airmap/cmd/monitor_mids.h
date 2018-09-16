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
