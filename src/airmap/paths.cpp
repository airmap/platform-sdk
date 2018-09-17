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
#include <airmap/paths.h>

#include <airmap/platform/interface.h>
#include <airmap/platform/standard_paths.h>

airmap::platform::Path airmap::paths::config_dir(Client::Version version) {
  auto p = airmap::platform::create_interface();
  switch (version) {
    case Client::Version::production:
      return p->standard_paths()
                 .path(platform::StandardPaths::Scope::user, platform::StandardPaths::Location::config)
                 .get() /
             "airmap" / "production";
    case Client::Version::staging:
      return p->standard_paths()
                 .path(platform::StandardPaths::Scope::user, platform::StandardPaths::Location::config)
                 .get() /
             "airmap" / "staging";
  }

  throw std::logic_error{"should not reach here"};
}

airmap::platform::Path airmap::paths::config_file(Client::Version version) {
  return config_dir(version) / "config.json";
}

airmap::platform::Path airmap::paths::token_file(Client::Version version) {
  return config_dir(version) / "token.json";
}
