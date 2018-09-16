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
//  client.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/net/mqtt/boost/client.h>

#include <mqtt/str_connect_return_code.hpp>

namespace asio = boost::asio;

namespace {

constexpr const char* component{"airmap::net::mqtt::boost::Client"};

}  // namespace

airmap::net::mqtt::boost::Client::Subscription::Subscription(Unsubscriber unsubscriber) : unsubscriber_{unsubscriber} {
}

airmap::net::mqtt::boost::Client::Subscription::~Subscription() {
  unsubscriber_();
}

std::shared_ptr<airmap::net::mqtt::boost::Client> airmap::net::mqtt::boost::Client::create(
    const std::shared_ptr<Logger>& logger, const std::shared_ptr<asio::io_service>& io_service,
    const std::shared_ptr<TlsClient>& mqtt_client) {
  return std::shared_ptr<Client>(new Client{logger, io_service, mqtt_client})->finalize();
}

airmap::net::mqtt::boost::Client::Client(const std::shared_ptr<Logger>& logger,
                                         const std::shared_ptr<asio::io_service>& io_service,
                                         const std::shared_ptr<TlsClient>& mqtt_client)
    : log_{logger}, io_service_{io_service}, mqtt_client_{mqtt_client} {
}

std::shared_ptr<airmap::net::mqtt::boost::Client> airmap::net::mqtt::boost::Client::finalize() {
  auto sp = shared_from_this();
  std::weak_ptr<Client> wp{sp};

  mqtt_client_->set_close_handler([wp]() {
    if (auto sp = wp.lock())
      sp->log_.infof(component, "connection to mqtt broker was closed");
  });

  mqtt_client_->set_error_handler([wp](const ::boost::system::error_code& ec) {
    if (auto sp = wp.lock())
      sp->log_.errorf(component, "failed to communicate with mqtt broker: %s", ec.message());
  });

  mqtt_client_->set_suback_handler([wp](std::uint16_t packet_id, std::vector<::boost::optional<std::uint8_t>> results) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received suback from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_puback_handler([wp](std::uint16_t packet_id) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received puback from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_pubrec_handler([wp](std::uint16_t packet_id) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received pubrec from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_pubcomp_handler([wp](std::uint16_t packet_id) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received pubcomp from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_publish_handler(
      [wp](std::uint8_t header, ::boost::optional<std::uint16_t> packet_id, std::string topic, std::string contents) {
        if (auto sp = wp.lock()) {
          sp->handle_publish(header, packet_id, topic, contents);
          return true;
        }
        return false;
      });

  return sp;
}

std::unique_ptr<airmap::net::mqtt::Client::Subscription> airmap::net::mqtt::boost::Client::subscribe(
    const std::string& topic, QualityOfService qos, PublishCallback cb) {
  auto translated = ::mqtt::qos::exactly_once;

  switch (qos) {
    case QualityOfService::at_least_once:
      translated = ::mqtt::qos::at_least_once;
      break;
    case QualityOfService::at_most_once:
      translated = ::mqtt::qos::at_most_once;
      break;
    case QualityOfService::exactly_once:
      translated = ::mqtt::qos::exactly_once;
      break;
  }

  auto id  = mqtt_client_->async_subscribe(topic, translated);
  auto itt = topic_map_.emplace(topic, cb);
  subscription_map_.emplace(id, itt);

  std::weak_ptr<Client> wp{shared_from_this()};
  std::unique_ptr<airmap::net::mqtt::Client::Subscription> result{new Subscription{[wp, id]() {
    if (auto sp = wp.lock()) {
      sp->unsubscribe(id);
    }
  }}};
  return result;
}

void airmap::net::mqtt::boost::Client::handle_publish(std::uint8_t, ::boost::optional<std::uint16_t>, std::string topic,
                                                      std::string contents) {
  log_.debugf(component, "received publish from mqtt broker for topic %s: size of contents %d", topic, contents.size());
  auto range = topic_map_.equal_range(topic);
  for (auto it = range.first; it != range.second; ++it) {
    it->second(topic, contents);
  }
}

void airmap::net::mqtt::boost::Client::unsubscribe(SubscriptionId subscription_id) {
  auto it = subscription_map_.find(subscription_id);

  if (it != subscription_map_.end()) {
    mqtt_client_->async_unsubscribe(it->second->first);
    topic_map_.erase(it->second);
    subscription_map_.erase(it);
  }
}
