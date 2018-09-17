// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
