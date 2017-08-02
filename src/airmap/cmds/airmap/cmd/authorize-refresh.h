#ifndef AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_REFERSH_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_REFERSH_H_

#include <airmap/authenticator.h>
#include <airmap/util/cli.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class AuthorizeRefresh : public util::cli::CommandWithFlagsAndAction {
 public:
  AuthorizeRefresh();

 private:
  std::string api_key_;
  Authenticator::RenewAuthentication::Params params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_REFERSH_H_