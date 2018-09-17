// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef AIRMAP_CMDS_AIRMAP_CMD_MONITOR_TRAFFIC_H_
#define AIRMAP_CMDS_AIRMAP_CMD_MONITOR_TRAFFIC_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/token.h>
#include <airmap/traffic.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class MonitorTraffic : public util::cli::CommandWithFlagsAndAction {
 public:
  MonitorTraffic();

 private:
  util::FormattingLogger log_{create_null_logger()};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<::airmap::Client> client_;
  std::shared_ptr<::airmap::Traffic::Monitor> monitor_;
  struct {
    Logger::Severity log_level{Logger::Severity::info};
    Required<ConfigFile> config_file;
    Required<TokenFile> token_file;
    Required<FlightId> flight_id;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_MONITOR_TRAFFIC_H_
