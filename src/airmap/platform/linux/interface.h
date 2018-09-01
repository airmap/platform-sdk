//
//  interface.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_PLATFORM_LINUX_INTERFACE_H_
#define AIRMAP_PLATFORM_LINUX_INTERFACE_H_

#include <airmap/platform/interface.h>
#include <airmap/platform/linux/standard_paths.h>

namespace airmap {
namespace platform {
namespace linux_ {

class Interface : public platform::Interface {
 public:
  // From platform::Interface
  platform::StandardPaths& standard_paths() override;

 private:
  StandardPaths standard_paths_;
};

}  // namespace linux_
}  // namespace platform
}  // namespace airmap

#endif  // AIRMAP_PLATFORM_LINUX_INTERFACE_H_
