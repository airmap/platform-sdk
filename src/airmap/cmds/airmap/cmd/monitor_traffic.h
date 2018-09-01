//
//  monitor_traffic.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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
