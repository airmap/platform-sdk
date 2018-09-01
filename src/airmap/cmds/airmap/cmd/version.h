//
//  version.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_VERSION_H_
#define AIRMAP_CMDS_AIRMAP_CMD_VERSION_H_

#include <airmap/util/cli.h>
#include <airmap/version.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Version : public util::cli::CommandWithFlagsAndAction {
 public:
  Version();
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_VERSION_H_
