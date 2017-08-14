#include <airmap/rest/traffic.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <nlohmann/json.hpp>

#include <fmt/printf.h>
#include <mqtt/client.hpp>

namespace ph = std::placeholders;
using json   = nlohmann::json;

namespace {
constexpr const char* component{"rest::Traffic::Monitor"};
}  // namespace

airmap::rest::Traffic::Monitor::Monitor(const std::shared_ptr<Logger>& logger, const std::string& flight_id,
                                        const std::shared_ptr<mqtt::Client>& client)
    : log_{logger}, flight_id_{flight_id}, mqtt_client_{client} {
  client->set_publish_handler(std::bind(&Monitor::handle_publish, this, ph::_1, ph::_2, ph::_3, ph::_4));
  client->set_suback_handler(std::bind(&Monitor::handle_suback, this, ph::_1, ph::_2));

  sa_subscription_id_ =
      client->async_subscribe(fmt::sprintf("uav/traffic/sa/%s", flight_id_), ::mqtt::qos::exactly_once);
  alert_subscription_id_ =
      client->async_subscribe(fmt::sprintf("uav/traffic/alert/%s", flight_id_), ::mqtt::qos::exactly_once);
}

void airmap::rest::Traffic::Monitor::subscribe(const std::shared_ptr<Subscriber>& subscriber) {
  subscribers_.insert(subscriber);
}

void airmap::rest::Traffic::Monitor::unsubscribe(const std::shared_ptr<Subscriber>& subscriber) {
  subscribers_.erase(subscriber);
}

bool airmap::rest::Traffic::Monitor::handle_suback(std::uint16_t packet_id,
                                                   std::vector<::boost::optional<std::uint8_t>> subs) {
  for (const auto& subscription : subs)
    log_.infof(component, "finished subscription: %s", subscription ? std::to_string(subscription.get()) : "error");
  return true;
}

bool airmap::rest::Traffic::Monitor::handle_publish(std::uint8_t, ::boost::optional<std::uint16_t>, std::string topic,
                                                    std::string contents) {
  log_.infof(component, "received publication for topic %s", topic);

  json j                     = json::parse(contents);
  std::vector<Update> update = j["traffic"];

  for (const auto& subscriber : subscribers_) {
    subscriber->handle_update(update);
  }

  return true;
}

airmap::rest::Traffic::Traffic(const std::shared_ptr<Logger>& logger, const std::string& host, std::uint16_t port,
                               Communicator& communicator)
    : log_{logger}, host_{host}, port_{port}, communicator_{communicator} {
}

void airmap::rest::Traffic::monitor(const Monitor::Params& params, const Monitor::Callback& cb) {
  communicator_.connect_to_mqtt_broker(
      host_, port_, params.flight_id, params.authorization, [ logger = log_.logger(), cb, params ](const auto& result) {
        if (result) {
          cb(Monitor::Result{std::make_shared<Monitor>(logger, params.flight_id, result.value())});
        } else {
          cb(Monitor::Result{result.error()});
        }
      });
}
