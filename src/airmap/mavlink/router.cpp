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
#include <airmap/mavlink/router.h>

airmap::mavlink::Router::Router(const std::initializer_list<std::shared_ptr<Route>>& routes) : routes_{routes} {
}

void airmap::mavlink::Router::route(const mavlink_message_t& message) {
  for (const auto& route : routes_)
    route->process(message);
}

void airmap::mavlink::Router::start() {
  for (const auto& route : routes_)
    route->start();
}

void airmap::mavlink::Router::stop() {
  for (const auto& route : routes_)
    route->stop();
}
