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
//  service.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/monitor/grpc/service.h>

#include <airmap/codec/grpc/traffic.h>

namespace {
constexpr const char* component{"airmap::monitor::grpc::Service"};
}  // namespace

airmap::monitor::grpc::Service::Service(const std::shared_ptr<Logger>& logger,
                                        const std::shared_ptr<Traffic::Monitor>& traffic_monitor)
    : log_{logger}, traffic_monitor_{traffic_monitor} {
}

::grpc::Service& airmap::monitor::grpc::Service::instance() {
  return async_monitor_;
}

void airmap::monitor::grpc::Service::start(::grpc::ServerCompletionQueue& cq) {
  log_.infof(component, "starting to serve grpc.airmap.Monitor service");
  ConnectToUpdates::start_listening(log_.logger(), &cq, &async_monitor_, traffic_monitor_);
}

airmap::monitor::grpc::Service::ConnectToUpdates::Subscriber::Subscriber(ConnectToUpdates* invocation)
    : invocation_{invocation} {
}

void airmap::monitor::grpc::Service::ConnectToUpdates::Subscriber::handle_update(
    airmap::Traffic::Update::Type type, const std::vector<airmap::Traffic::Update>& updates) {
  ::grpc::airmap::monitor::Update u;

  for (const auto& update : updates) {
    codec::grpc::encode(*u.add_traffic(), update);
  }

  invocation_->write(u);
}

void airmap::monitor::grpc::Service::ConnectToUpdates::start_listening(
    const std::shared_ptr<Logger>& logger, ::grpc::ServerCompletionQueue* completion_queue,
    ::grpc::airmap::monitor::Monitor::AsyncService* async_monitor,
    const std::shared_ptr<Traffic::Monitor>& traffic_monitor) {
  new ConnectToUpdates(logger, completion_queue, async_monitor, traffic_monitor);
}

airmap::monitor::grpc::Service::ConnectToUpdates::ConnectToUpdates(
    const std::shared_ptr<Logger>& logger, ::grpc::ServerCompletionQueue* completion_queue,
    ::grpc::airmap::monitor::Monitor::AsyncService* async_monitor,
    const std::shared_ptr<Traffic::Monitor>& traffic_monitor)
    : log_{logger},
      completion_queue_{completion_queue},
      async_monitor_{async_monitor},
      traffic_monitor_{traffic_monitor},
      responder_{&server_context_} {
  async_monitor_->RequestConnectToUpdates(&server_context_, &parameters_, &responder_, completion_queue_,
                                          completion_queue_, this);
}

void airmap::monitor::grpc::Service::ConnectToUpdates::write(const ::grpc::airmap::monitor::Update& update) {
  responder_.Write(update, this);
}

void airmap::monitor::grpc::Service::ConnectToUpdates::proceed(bool result) {
  log_.debugf(component, "ConnectToUpdates::proceed: (%s, %s)", state_, result ? "true" : "false");
  if (state_ == State::ready) {
    start_listening(log_.logger(), completion_queue_, async_monitor_, traffic_monitor_);
    if (result) {
      state_                      = State::streaming;
      traffic_monitor_subscriber_ = std::make_shared<Subscriber>(this);
      traffic_monitor_->subscribe(traffic_monitor_subscriber_);
    } else {
      state_ = State::finished;
      responder_.Finish(::grpc::Status::CANCELLED, this);
    }
  } else if (state_ == State::streaming) {
    // We have encountered an error and cancel the streaming.
    if (!result) {
      // After the subscriber has been unsubscribed, no more updates will come in
      // from the traffic monitor. With that, we are good to clean up ourselves.
      traffic_monitor_->unsubscribe(traffic_monitor_subscriber_);
      state_ = State::finished;
      responder_.Finish(::grpc::Status::CANCELLED, this);
    }
  } else if (state_ == State::finished) {
    delete this;
  }
}