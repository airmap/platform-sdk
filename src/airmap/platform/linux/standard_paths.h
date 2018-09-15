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
//  standard_paths.h
//  AirMap Platform SDK
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
