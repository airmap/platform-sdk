#ifndef AIRMAP_CMDS_AIRMAP_CMD_PILOT_H_
#define AIRMAP_CMDS_AIRMAP_CMD_PILOT_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/pilots.h>
#include <airmap/token.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Pilot : public util::cli::CommandWithFlagsAndAction {
 public:
  Pilot();

 private:
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;
  using PilotId         = util::TaggedString<util::tags::MustNotBeEmpty>;

  void handle_authenticated_pilot_result(const Pilots::Authenticated::Result& result, ConstContextRef context);
  void handle_for_id_pilot_result(const Pilots::ForId::Result& result, ConstContextRef context);
  void handle_aircrafts_result(const ::airmap::Pilot& pilot, const Pilots::Aircrafts::Result& result,
                               ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<::airmap::Client> client_;
  Required<ConfigFile> config_file_;
  Required<TokenFile> token_file_;
  Optional<Token> token_;
  Optional<PilotId> pilot_id_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_PILOT_H_
