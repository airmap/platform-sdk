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
#ifndef AIRMAP_CMDS_AIRMAP_CMD_PLAN_FLIGHT_H_
#define AIRMAP_CMDS_AIRMAP_CMD_PLAN_FLIGHT_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/context.h>
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

class PlanFlight : public util::cli::CommandWithFlagsAndAction {
 public:
  PlanFlight();

 private:
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;
  using PlanFile        = util::TaggedString<util::tags::MustNotBeEmpty>;

  void handle_flight_plan_create_result(const FlightPlans::Create::Result& result, ConstContextRef context);
  void handle_flight_plan_update_result(const FlightPlans::Update::Result& result, ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<::airmap::Client> client_;
  Optional<ConfigFile> config_file_;
  Optional<TokenFile> token_file_;
  Optional<PlanFile> plan_file_;
  bool update_{false};
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_PLAN_FLIGHT_H_
