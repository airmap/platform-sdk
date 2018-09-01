//
//  standard_paths.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_PLATFORM_LINUX_STANDARD_PATHS_H_
#define AIRMAP_PLATFORM_LINUX_STANDARD_PATHS_H_

#include <airmap/platform/standard_paths.h>

namespace airmap {
namespace platform {
namespace linux_ {

class StandardPaths : public platform::StandardPaths {
 public:
  Optional<Path> path(Scope scope, Location location);
};

}  // namespace linux_
}  // namespace platform
}  // namespace airmap

#endif  // AIRMAP_PLATFORM_LINUX_STANDARD_PATHS_H_
