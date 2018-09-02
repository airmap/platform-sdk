//
//  report_weather.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_REPORT_WEATHER_H_
#define AIRMAP_CMDS_AIRMAP_CMD_REPORT_WEATHER_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/advisory.h>
#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class ReportWeather : public util::cli::CommandWithFlagsAndAction {
 public:
  ReportWeather();

 private:
  void handle_report_weather_result(const Advisory::ReportWeather::Result& result);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Required<ConfigFile> config_file_;
  Advisory::ReportWeather::Parameters params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_REPORT_WEATHER_H_
