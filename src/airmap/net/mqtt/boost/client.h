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
#ifndef AIRMAP_NET_MQTT_BOOST_CLIENT_H_
#define AIRMAP_NET_MQTT_BOOST_CLIENT_H_

#include <airmap/net/mqtt/client.h>

#include <airmap/logger.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <mqtt/client.hpp>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace airmap {
namespace net {
namespace mqtt {
namespace boost {

class Client : public mqtt::Client, public std::enable_shared_from_this<Client> {
 public:
  using TlsClient    = ::mqtt::client<::mqtt::tcp_endpoint<::boost::asio::ssl::stream<::boost::asio::ip::tcp::socket>,
                                                        ::boost::asio::io_service::strand>>;
  using Unsubscriber = std::function<void()>;

  class Subscription : public mqtt::Client::Subscription {
   public:
    explicit Subscription(Unsubscriber unsubscriber);
    ~Subscription();

   private:
    Unsubscriber unsubscriber_;
  };

  static std::shared_ptr<Client> create(const std::shared_ptr<Logger>& logger,
                                        const std::shared_ptr<::boost::asio::io_service>& io_service,
                                        const std::shared_ptr<TlsClient>& mqtt_client);

  // From airmap::net::mqtt::Client
  std::unique_ptr<mqtt::Client::Subscription> subscribe(const std::string& topic, QualityOfService qos,
                                                        PublishCallback cb) override;

 private:
  using Topic            = std::string;
  using TopicMap         = std::unordered_multimap<Topic, PublishCallback>;
  using TopicMapIterator = typename TopicMap::iterator;
  using SubscriptionId   = std::uint8_t;
  using SubscriptionMap  = std::unordered_map<SubscriptionId, TopicMapIterator>;

  /// Initializes the Client instance with mqtt_client
  explicit Client(const std::shared_ptr<Logger>& logger, const std::shared_ptr<::boost::asio::io_service>& io_service,
                  const std::shared_ptr<TlsClient>& mqtt_client);
  std::shared_ptr<Client> finalize();

  void handle_publish(std::uint8_t, ::boost::optional<std::uint16_t>, std::string topic, std::string contents);
  void unsubscribe(SubscriptionId subscription_id);

  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  std::shared_ptr<TlsClient> mqtt_client_;
  SubscriptionMap subscription_map_;
  TopicMap topic_map_;
};

}  // namespace boost
}  // namespace mqtt
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_MQTT_CLIENT_H_
