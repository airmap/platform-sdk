//
//  end_flight.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_END_FLIGHT_H_
#define AIRMAP_CMDS_AIRMAP_CMD_END_FLIGHT_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/telemetry.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class EndFlight : public util::cli::CommandWithFlagsAndAction {
 public:
  EndFlight();

 private:
  util::FormattingLogger log_{create_null_logger()};
  struct {
    Client::Version version{Client::Version::production};
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

#endif  // AIRMAP_CMDS_AIRMAP_CMD_END_FLIGHT_H_
