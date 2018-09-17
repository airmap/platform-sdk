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
#include <airmap/platform/linux/standard_paths.h>

#include <airmap/platform/linux/xdg.h>

airmap::Optional<airmap::platform::Path> airmap::platform::linux_::StandardPaths::path(Scope scope, Location location) {
  switch (location) {
    case Location::cache:
      switch (scope) {
        case Scope::system:
          return Optional<Path>{};
        case Scope::user:
          return xdg::cache().home();
      }
      break;
    case Location::config:
      switch (scope) {
        case Scope::system:
          return xdg::config().dirs().front();
        case Scope::user:
          return xdg::config().home();
      }
      break;
    case Location::data:
      switch (scope) {
        case Scope::system:
          return xdg::data().dirs().front();
        case Scope::user:
          return xdg::data().home();
      }
      break;
    case Location::runtime:
      switch (scope) {
        case Scope::system:
          return xdg::runtime().dir();
        case Scope::user:
          return Optional<Path>{};
      }
      break;
  }

  throw std::logic_error{"should not reach here"};
}
