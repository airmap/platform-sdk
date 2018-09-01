//
//  evaluate_rulesets.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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
