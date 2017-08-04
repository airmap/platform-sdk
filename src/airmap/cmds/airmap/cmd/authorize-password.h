#ifndef AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_PASSWORD_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_PASSWORD_H_

#include <airmap/authenticator.h>
#include <airmap/util/cli.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class AuthorizePassword : public util::cli::CommandWithFlagsAndAction {
 public:
  AuthorizePassword();

 private:
  std::string api_key_;
  Authenticator::AuthenticateWithPassword::Params params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_PASSWORD_H_
