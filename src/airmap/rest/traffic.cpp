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

std::shared_ptr<airmap::rest::Traffic::Monitor> airmap::rest::Traffic::Monitor::create(
    const std::shared_ptr<Logger>& logger, const std::string& flight_id, const std::shared_ptr<mqtt::Client>& client) {
  return std::shared_ptr<Monitor>(new Monitor{logger, flight_id, client})->finalize();
}

airmap::rest::Traffic::Monitor::Monitor(const std::shared_ptr<Logger>& logger, const std::string& flight_id,
                                        const std::shared_ptr<mqtt::Client>& client)
    : log_{logger}, flight_id_{flight_id}, mqtt_client_{client} {
}

std::shared_ptr<airmap::rest::Traffic::Monitor> airmap::rest::Traffic::Monitor::finalize() {
  auto sp = shared_from_this();
  std::weak_ptr<Monitor> wp{sp};

  auto cb = [wp](const std::string& topic, const std::string& contents) {
    if (auto sp = wp.lock())
      sp->handle_publish(topic, contents);
  };
  auto qos = mqtt::QualityOfService::exactly_once;

  sa_sub_    = mqtt_client_->subscribe(fmt::sprintf("uav/traffic/sa/%s", flight_id_), qos, cb);
  alert_sub_ = mqtt_client_->subscribe(fmt::sprintf("uav/traffic/alert/%s", flight_id_), qos, cb);

  return sp;
}

void airmap::rest::Traffic::Monitor::subscribe(const std::shared_ptr<Subscriber>& subscriber) {
  subscribers_.insert(subscriber);
}

void airmap::rest::Traffic::Monitor::unsubscribe(const std::shared_ptr<Subscriber>& subscriber) {
  subscribers_.erase(subscriber);
}

void airmap::rest::Traffic::Monitor::handle_publish(const std::string& topic, const std::string& contents) {
  log_.infof(component, "received publish for topic %s", topic);

  json j                     = json::parse(contents);
  std::vector<Update> update = j["traffic"];

  for (const auto& subscriber : subscribers_) {
    subscriber->handle_update(update);
  }
}

airmap::rest::Traffic::Traffic(const std::shared_ptr<Logger>& logger, const std::string& host, std::uint16_t port,
                               Communicator& communicator)
    : log_{logger}, host_{host}, port_{port}, communicator_{communicator} {
}

void airmap::rest::Traffic::monitor(const Monitor::Params& params, const Monitor::Callback& cb) {
  communicator_.connect_to_mqtt_broker(
      host_, port_, params.flight_id, params.authorization, [ logger = log_.logger(), cb, params ](const auto& result) {
        if (result) {
          cb(Monitor::Result{Monitor::create(logger, params.flight_id, result.value())});
        } else {
          cb(Monitor::Result{result.error()});
        }
      });
}
