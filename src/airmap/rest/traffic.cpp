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
#include <airmap/rest/traffic.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

#include <mqtt/client.hpp>

namespace fmt = airmap::util::fmt;
namespace ph  = std::placeholders;
using json    = nlohmann::json;

namespace {
constexpr const char* component{"rest::Traffic::Monitor"};
}  // namespace

std::shared_ptr<airmap::rest::Traffic::Monitor> airmap::rest::Traffic::Monitor::create(
    const std::string& flight_id, const std::shared_ptr<net::mqtt::Client>& client) {
  return std::shared_ptr<Monitor>(new Monitor{flight_id, client})->finalize();
}

airmap::rest::Traffic::Monitor::Monitor(const std::string& flight_id, const std::shared_ptr<net::mqtt::Client>& client)
    : flight_id_{flight_id}, mqtt_client_{client} {
}

std::shared_ptr<airmap::rest::Traffic::Monitor> airmap::rest::Traffic::Monitor::finalize() {
  auto sp = shared_from_this();
  std::weak_ptr<Monitor> wp{sp};

  auto cb = [wp](const std::string& topic, const std::string& contents) {
    if (auto sp = wp.lock())
      sp->handle_publish(topic, contents);
  };
  auto qos = net::mqtt::QualityOfService::exactly_once;

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
  std::vector<Update> update = json::parse(contents)["traffic"];

  Traffic::Update::Type type{Traffic::Update::Type::unknown};

  if (topic.find("uav/traffic/sa") == 0) {
    type = Traffic::Update::Type::situational_awareness;
  } else if (topic.find("uav/traffic/alert") == 0) {
    type = Traffic::Update::Type::alert;
  }

  for (const auto& subscriber : subscribers_) {
    subscriber->handle_update(type, update);
  }
}

airmap::rest::Traffic::Traffic(const std::shared_ptr<net::mqtt::Broker>& broker) : broker_{broker} {
}

void airmap::rest::Traffic::monitor(const Monitor::Params& params, const Monitor::Callback& cb) {
  broker_->connect({params.flight_id, params.authorization}, [params, cb](const auto& result) {
    if (result) {
      cb(Monitor::Result{Monitor::create(params.flight_id, result.value())});
    } else {
      cb(Monitor::Result{result.error()});
    }
  });
}
