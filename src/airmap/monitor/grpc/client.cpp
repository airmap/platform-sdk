#include <airmap/monitor/grpc/client.h>

#include <grpc++/grpc++.h>

airmap::monitor::grpc::Client::Client(const Configuration& configuration, const std::shared_ptr<Context>& context)
    : context_{context},
      completion_queue_worker_{[this]() {
        bool ok   = false;
        void* tag = nullptr;

        while (completion_queue_.Next(&tag, &ok)) {
          if (auto mi = static_cast<ConnectToUpdatesInvocation*>(tag)) {
            mi->proceed(ok);
          }

          if (!ok)
            break;
        }
      }},
      stub_{std::make_shared<::grpc::airmap::monitor::Monitor::Stub>(
          ::grpc::CreateChannel(configuration.endpoint, ::grpc::InsecureChannelCredentials()))} {
}

airmap::monitor::grpc::Client::~Client() {
  completion_queue_.Shutdown();

  if (completion_queue_worker_.joinable()) {
    completion_queue_worker_.join();
  }
}

void airmap::monitor::grpc::Client::connect_to_updates(const ConnectToUpdates::Callback& cb) {
  ConnectToUpdatesInvocation::start(stub_, &completion_queue_, cb, context_);
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

void airmap::monitor::grpc::Client::ConnectToUpdatesInvocation::start(const std::shared_ptr<Stub>& stub,
                                                                      ::grpc::CompletionQueue* completion_queue,
                                                                      const ConnectToUpdates::Callback& cb,
                                                                      const std::shared_ptr<Context>& context) {
  new ConnectToUpdatesInvocation{stub, completion_queue, cb, context};
}

airmap::monitor::grpc::Client::ConnectToUpdatesInvocation::ConnectToUpdatesInvocation(
    const std::shared_ptr<Stub>& stub, ::grpc::CompletionQueue* completion_queue, const ConnectToUpdates::Callback& cb,
    const std::shared_ptr<Context>& context)
    : stub_{stub},
      completion_queue_{completion_queue},
      cb_{cb},
      context_{context},
      stream_{stub_->AsyncConnectToUpdates(&client_context_, Parameters{}, completion_queue_, this)} {
}

void airmap::monitor::grpc::Client::ConnectToUpdatesInvocation::proceed(bool result) {
  switch (state_) {
    case State::connecting:
      if (!result) {
        context_->dispatch([cb = cb_]() { cb(ConnectToUpdates::Result{Error{"failed to connect to updates"}}); });
        state_ = State::finished;
        stream_->Finish(&status_, this);
      } else {
        update_stream_ = std::make_shared<UpdateStreamImpl>();
        context_->dispatch([ cb = cb_, us = update_stream_ ]() { cb(ConnectToUpdates::Result{us}); });
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
          const auto& update = element_.traffic(i);
          Traffic::Update tu;

          tu.aircraft_id = update.aircraft_id();

          if (update.has_track()) {
            tu.id = update.track().as_string();
          } else {
            tu.id = "n/a";
          }

          if (update.has_position()) {
            tu.latitude  = update.position().has_latitude() ? update.position().latitude().value() : 0;
            tu.longitude = update.position().has_longitude() ? update.position().longitude().value() : 0;
          }

          if (update.has_ground_speed()) {
            tu.ground_speed = update.ground_speed().value();
          } else {
            tu.ground_speed = 0;
          }

          if (update.has_heading()) {
            tu.heading = update.heading().value();
          } else {
            tu.heading = 0;
          }

          if (update.has_direction()) {
            tu.direction = update.direction().value();
          } else {
            tu.direction = 0;
          }

          if (update.has_recorded()) {
            auto micros = update.recorded().seconds() * 1000 * 1000 + update.recorded().nanos() / 1000;
            tu.recorded = from_microseconds_since_epoch(Microseconds{micros});
          } else {
            tu.recorded = Clock::universal_time();
          }

          if (update.has_generated()) {
            auto micros  = update.generated().seconds() * 1000 * 1000 + update.generated().nanos() / 1000;
            tu.timestamp = from_microseconds_since_epoch(Microseconds{micros});
          } else {
            tu.timestamp = Clock::universal_time();
          }

          u.traffic.push_back(tu);
        }

        context_->dispatch([ us = update_stream_, u ]() { us->write_update(u); });
        stream_->Read(&element_, this);
      }
      break;
    case State::finished:
      delete this;
      break;
  }
}