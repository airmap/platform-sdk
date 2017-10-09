#ifndef AIRMAP_CMDS_AIRMAP_CMD_GET_RULES_H_
#define AIRMAP_CMDS_AIRMAP_CMD_GET_RULES_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
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

class GetRules : public util::cli::CommandWithFlagsAndAction {
 public:
  GetRules();

 private:
 	
  //using GeometryFile = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Required<ConfigFile> config_file_;
  Optional<std::string> rulesets_;
  RuleSets::GetRules::Parameters params_;
  
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif // AIRMAP_CMDS_AIRMAP_CMD_GET_RULES_H_
