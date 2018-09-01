//
//  traffic.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_REST_TRAFFIC_H_
#define AIRMAP_REST_TRAFFIC_H_

#include <airmap/traffic.h>

#include <airmap/logger.h>
#include <airmap/net/mqtt/broker.h>
#include <airmap/net/mqtt/client.h>

#include <set>
#include <vector>

namespace airmap {
namespace rest {

class Traffic : public airmap::Traffic {
 public:
  class Monitor : public airmap::Traffic::Monitor, public std::enable_shared_from_this<Monitor> {
   public:
    static std::shared_ptr<Monitor> create(const std::string& flight_id,
                                           const std::shared_ptr<net::mqtt::Client>& client);

    void subscribe(const std::shared_ptr<Subscriber>& subscriber) override;
    void unsubscribe(const std::shared_ptr<Subscriber>& subscriber) override;

   private:
    explicit Monitor(const std::string& flight_id, const std::shared_ptr<net::mqtt::Client>& client);
    std::shared_ptr<Monitor> finalize();

    void handle_publish(const std::string& topic, const std::string& contents);

    std::string flight_id_;
    std::shared_ptr<net::mqtt::Client> mqtt_client_;
    std::unique_ptr<net::mqtt::Client::Subscription> sa_sub_;
    std::unique_ptr<net::mqtt::Client::Subscription> alert_sub_;
    std::set<std::shared_ptr<Subscriber>> subscribers_;
    std::uint8_t sa_subscription_id_;
  };

  explicit Traffic(const std::shared_ptr<net::mqtt::Broker>& broker);
  void monitor(const Monitor::Params& params, const Monitor::Callback& cb) override;

 private:
  std::shared_ptr<net::mqtt::Broker> broker_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_TRAFFIC_H_
