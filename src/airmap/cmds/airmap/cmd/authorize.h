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
  struct {
    Authenticator::AuthenticateAnonymously::Params anon;
    Authenticator::AuthenticateWithPassword::Params passwd;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_H_