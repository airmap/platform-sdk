//
//  render_briefing.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_RENDER_BRIEFING_H_
#define AIRMAP_CMDS_AIRMAP_CMD_RENDER_BRIEFING_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/evaluation.h>
#include <airmap/flight_plans.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class RenderBriefing : public util::cli::CommandWithFlagsAndAction {
 public:
  RenderBriefing();

 private:
  using FlightPlanId = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Optional<ConfigFile> config_file_;
  Optional<TokenFile> token_file_;
  Optional<FlightPlanId> flight_plan_id_;
  FlightPlans::RenderBriefing::Parameters parameters_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_RENDER_BRIEFING_H_
