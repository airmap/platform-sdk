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
#ifndef AIRMAP_NET_MQTT_BROKER_H_
#define AIRMAP_NET_MQTT_BROKER_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/error.h>
#include <airmap/outcome.h>

#include <functional>
#include <memory>
#include <string>

namespace airmap {
namespace net {
namespace mqtt {

class Client;

class Broker : public DoNotCopyOrMove {
 public:
  using ConnectResult   = Outcome<std::shared_ptr<Client>, Error>;
  using ConnectCallback = std::function<void(const ConnectResult&)>;

  struct Credentials {
    std::string username;
    std::string password;
  };

  virtual void connect(const Credentials& credentials, const ConnectCallback& cb) = 0;

 protected:
  Broker() = default;
};

}  // namespace mqtt
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_MQTT_BROKER_H_
