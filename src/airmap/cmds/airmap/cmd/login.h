#ifndef AIRMAP_CMDS_AIRMAP_CMD_LOGIN_H_
#define AIRMAP_CMDS_AIRMAP_CMD_LOGIN_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/credentials.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/status.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Login : public util::cli::CommandWithFlagsAndAction {
 public:
  Login();

 private:
  void request_authentication(const Credentials& credentials);
  void handle_result_for_authentication_with_password(const Authenticator::AuthenticateWithPassword::Result& result);
  void handle_result_for_anonymous_authentication(const Authenticator::AuthenticateAnonymously::Result& result);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Optional<ConfigFile> config_file_;
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<Client> client_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_LOGIN_H_
