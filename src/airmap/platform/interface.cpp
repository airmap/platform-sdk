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
#include <airmap/platform/interface.h>

#if defined(AIRMAP_PLATFORM_LINUX)
#include <airmap/platform/linux/interface.h>
std::shared_ptr<airmap::platform::Interface> airmap::platform::create_interface() {
  return std::make_shared<airmap::platform::linux_::Interface>();
}
#endif  // AIRMAP_PLATFORM_LINUX

#if defined(AIRMAP_PLATFORM_NULL)
#include <airmap/platform/null/interface.h>
std::shared_ptr<airmap::platform::Interface> airmap::platform::create_interface() {
  return std::make_shared<airmap::platform::null::Interface>();
}
#endif  // AIRMAP_PLATFORM_NULL
