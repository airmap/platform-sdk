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
#ifndef AIRMAP_CMDS_AIRMAP_CMD_EVALUATE_RULESETS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_EVALUATE_RULESETS_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/evaluation.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/rulesets.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class EvaluateRuleSets : public util::cli::CommandWithFlagsAndAction {
 public:
  EvaluateRuleSets();

 private:
  using EvaluationFile = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Required<ConfigFile> config_file_;
  Required<EvaluationFile> evaluation_file_;
  RuleSets::EvaluateRules::Parameters params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_EVALUATE_RULESETS_H_
