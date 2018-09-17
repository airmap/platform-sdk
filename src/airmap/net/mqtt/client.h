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
#ifndef AIRMAP_NET_MQTT_CLIENT_H_
#define AIRMAP_NET_MQTT_CLIENT_H_

#include <airmap/do_not_copy_or_move.h>

#include <functional>
#include <memory>
#include <string>

namespace airmap {
namespace net {
namespace mqtt {

/// QualityOfService models all known
/// QoS values supported by mqtt.
enum class QualityOfService { at_least_once, at_most_once, exactly_once };

/// Client models access to an MQTT broker, and enables
/// calling code to subscribe to topics.
class Client : DoNotCopyOrMove {
 public:
  /// PublishCallback is invoked for incoming publishs.
  /// @param topic the topic of the publish
  /// @param contents the contents of the publish
  using PublishCallback = std::function<void(const std::string&, const std::string&)>;

  /// An instance of Subscription models an individual subscription to a
  /// topic. Deleting the instance removes the specific subscription.
  class Subscription : DoNotCopyOrMove {
   protected:
    Subscription() = default;
  };

  /// subscribe subscribes the caller to 'topic', with 'qos', invoking cb
  /// for incoming publishs of 'topic'.
  virtual std::unique_ptr<Subscription> subscribe(const std::string& topic, QualityOfService qos,
                                                  PublishCallback cb) = 0;

 protected:
  Client() = default;
};

}  // namespace mqtt
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_MQTT_CLIENT_H_
