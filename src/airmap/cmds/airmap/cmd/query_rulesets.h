//
//  query_rulesets.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_QUERY_RULESETS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_QUERY_RULESETS_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/context.h>
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

class QueryRuleSets : public util::cli::CommandWithFlagsAndAction {
 public:
  QueryRuleSets();

 private:
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;
  using GeometryFile    = util::TaggedString<util::tags::MustNotBeEmpty>;
  using RuleSetId       = util::TaggedString<util::tags::MustNotBeEmpty>;

  void handle_ruleset_for_id_result(const RuleSets::ForId::Result& result, ConstContextRef context);
  void handle_ruleset_search_result(const RuleSets::Search::Result& result, ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<::airmap::Client> client_;
  Required<ConfigFile> config_file_;
  Optional<GeometryFile> geometry_file_;
  Optional<RuleSetId> ruleset_id_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_QUERY_RULESETS_H_
