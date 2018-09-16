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
//  user_agent.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/net/http/user_agent.h>

#include <airmap/util/fmt.h>
#include <airmap/version.h>

std::string airmap::net::http::user_agent() {
  return util::fmt::sprintf("AirMap/%d.%d.%d", Version::current().major, Version::current().minor,
                            Version::current().patch);
}
