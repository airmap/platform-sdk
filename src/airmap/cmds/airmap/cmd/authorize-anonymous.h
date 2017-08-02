#ifndef AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_ANONYMOUS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_ANONYMOUS_H_

#include <airmap/authenticator.h>
#include <airmap/util/cli.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class AuthorizeAnonymous : public util::cli::CommandWithFlagsAndAction {
 public:
  AuthorizeAnonymous();

 private:
  std::string api_key_;
  Authenticator::AuthenticateAnonymously::Params params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_ANONYMOUS_H_