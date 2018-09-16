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
//  broker.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_NET_MQTT_BOOST_BROKER_H_
#define AIRMAP_NET_MQTT_BOOST_BROKER_H_

#include <airmap/net/mqtt/broker.h>

#include <airmap/logger.h>

#include <boost/asio.hpp>

#include <string>

namespace airmap {
namespace net {
namespace mqtt {
namespace boost {

class Broker : public mqtt::Broker {
 public:
  explicit Broker(const std::string& host, std::uint16_t port, const std::shared_ptr<Logger>& logger,
                  const std::shared_ptr<::boost::asio::io_service>& io_service);
  void connect(const Credentials& credentials, const ConnectCallback& cb);

 private:
  std::shared_ptr<Logger> logger_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  std::string host_;
  std::uint16_t port_;
};

}  // namespace boost
}  // namespace mqtt
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_MQTT_BOOST_BROKER_H_
