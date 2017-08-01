#ifndef AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_H_

#include <airmap/authenticator.h>
#include <airmap/util/cli.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Authorize : public util::cli::CommandWithFlagsAndAction {
 public:
  Authorize();

 private:
  std::string api_key_;
  Authenticator::AuthenticateAnonymously::Params params_;
  Authenticator::AuthenticateWithPassword::Params passParams_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_H_