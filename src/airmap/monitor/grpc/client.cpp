//
//  client.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/monitor/grpc/client.h>

#include <airmap/codec.h>

#include <grpc++/grpc++.h>

namespace {
constexpr const char* component{"airmap::monitor::grpc::Client"};
}  // namespace

airmap::monitor::grpc::Client::Client(const Configuration& configuration, const std::shared_ptr<Context>& context)
    : log_{configuration.logger},
      context_{context},
      executor_worker_{[this]() { executor_.run(); }},
      stub_{std::make_shared<::grpc::airmap::monitor::Monitor::Stub>(
          ::grpc::CreateChannel(configuration.endpoint, ::grpc::InsecureChannelCredentials()))} {
}

airmap::monitor::grpc::Client::~Client() {
  executor_.stop();

  if (executor_worker_.joinable()) {
    executor_worker_.join();
  }
}

void airmap::monitor::grpc::Client::connect_to_updates(const ConnectToUpdates::Callback& cb) {
  executor_.invoke_method([this, cb](auto cq) {
    log_.debugf(component, "starting request for grpc.airmap.ConnectToUpdates");
    ConnectToUpdatesInvocation::start(log_.logger(), stub_, cq, cb, context_);
  });
}

void airmap::monitor::grpc::Client::UpdateStreamImpl::write_update(const Update& update) {
  std::set<std::shared_ptr<Receiver>> copy;

  {
    std::lock_guard<std::mutex> lg{receivers_guard_};
    copy = receivers_;
  }

  for (const auto& receiver : copy)
    receiver->handle_update(update);
}

// From UpdateStream
void airmap::monitor::grpc::Client::UpdateStreamImpl::subscribe(const std::shared_ptr<Receiver>& receiver) {
  std::lock_guard<std::mutex> lg{receivers_guard_};
  receivers_.insert(receiver);
}

void airmap::monitor::grpc::Client::UpdateStreamImpl::unsubscribe(const std::shared_ptr<Receiver>& receiver) {
  std::lock_guard<std::mutex> lg{receivers_guard_};
  receivers_.erase(receiver);
}

void airmap::monitor::grpc::Client::ConnectToUpdatesInvocation::start(const std::shared_ptr<Logger>& logger,
                                                                      const std::shared_ptr<Stub>& stub,
                                                                      ::grpc::CompletionQueue* completion_queue,
                                                                      const ConnectToUpdates::Callback& cb,
                                                                      const std::shared_ptr<Context>& context) {
  new ConnectToUpdatesInvocation{logger, stub, completion_queue, cb, context};
}

airmap::monitor::grpc::Client::ConnectToUpdatesInvocation::ConnectToUpdatesInvocation(
    const std::shared_ptr<Logger>& logger, const std::shared_ptr<Stub>& stub, ::grpc::CompletionQueue* completion_queue,
    const ConnectToUpdates::Callback& cb, const std::shared_ptr<Context>& context)
    : log_{logger},
      stub_{stub},
      completion_queue_{completion_queue},
      cb_{cb},
      context_{context},
      stream_{stub_->AsyncConnectToUpdates(&client_context_, Parameters{}, completion_queue_, this)} {
}

void airmap::monitor::grpc::Client::ConnectToUpdatesInvocation::proceed(bool result) {
  log_.debugf(component, "ConnectToUpdatesInvocation::proceed: (%s, %s)", state_, result ? "true" : "false");
  switch (state_) {
    case State::ready:
      if (!result) {
        context_->dispatch([cb = cb_]() { cb(ConnectToUpdates::Result{Error{"failed to connect to updates"}}); });
        state_ = State::finished;
        stream_->Finish(&status_, this);
      } else {
        update_stream_ = std::make_shared<UpdateStreamImpl>();
        context_->dispatch([cb = cb_, us = update_stream_]() { cb(ConnectToUpdates::Result{us}); });
        state_ = State::streaming;
        stream_->Read(&element_, this);
      }
      break;
    case State::streaming:
      if (!result) {
        state_ = State::finished;
        stream_->Finish(&status_, this);
      } else {
        Update u;

        for (int i = 0; i < element_.traffic_size(); i++) {
          Traffic::Update to;
          codec::grpc::decode(element_.traffic(i), to);
          u.traffic.push_back(to);
        }

        context_->dispatch([us = update_stream_, u]() { us->write_update(u); });
        stream_->Read(&element_, this);
      }
      break;
    case State::finished:
      delete this;
      break;
  }
}