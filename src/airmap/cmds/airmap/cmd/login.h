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
#include <airmap/token.h>
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
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;

  void renew_authentication(const Credentials& credentials, const Token& token, ConstContextRef context);
  void request_authentication(const Credentials& credentials, ConstContextRef context);
  void handle_result_for_authentication_with_password(const Authenticator::AuthenticateWithPassword::Result& result,
                                                      ConstContextRef context);
  void handle_result_for_anonymous_authentication(const Authenticator::AuthenticateAnonymously::Result& result,
                                                  ConstContextRef context);
  void handle_result_for_renewed_authentication(const Authenticator::RenewAuthentication::Result& result,
                                                const Token& previous_token, ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Optional<ConfigFile> config_file_;
  Optional<TokenFile> token_file_;
  bool renew_{false};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<Client> client_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_LOGIN_H_
