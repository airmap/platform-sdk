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
//  paths.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_PATHS_H_
#define AIRMAP_PATHS_H_

#include <airmap/client.h>
#include <airmap/platform/path.h>

namespace airmap {
namespace paths {

platform::Path config_dir(Client::Version version);
platform::Path config_file(Client::Version version);
platform::Path token_file(Client::Version version);

}  // namespace paths
}  // namespace airmap

#endif  // AIRMAP_PATHS_H_
