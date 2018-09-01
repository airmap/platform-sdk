//
//  init.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_INIT_H_
#define AIRMAP_CMDS_AIRMAP_CMD_INIT_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/credentials.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/version.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Init : public util::cli::CommandWithFlagsAndAction {
 public:
  Init();

 private:
  Client::Version version_{Client::Version::production};
  Credentials::Type credentials_type_{Credentials::Type::oauth};
  Optional<ConfigFile> config_file_;
  bool interactive_{true};
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_INIT_H_
