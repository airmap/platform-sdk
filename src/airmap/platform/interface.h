//
//  interface.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_PLATFORM_INTERFACE_H_
#define AIRMAP_PLATFORM_INTERFACE_H_

#include <airmap/do_not_copy_or_move.h>

#include <memory>

namespace airmap {
namespace platform {

class StandardPaths;

class Interface : DoNotCopyOrMove {
 public:
  virtual StandardPaths& standard_paths() = 0;

 protected:
  Interface() = default;
};

std::shared_ptr<Interface> create_interface();

}  // namespace platform
}  // namespace airmap

#endif  // AIRMAP_PLATFORM_INTERFACE_H_
