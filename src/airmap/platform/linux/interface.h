/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  interface.h
//  AirMap Platform SDK
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
