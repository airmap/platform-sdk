#include <airmap/monitor/grpc/service.h>

#include "grpc/airmap/traffic.pb.h"

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
    ::grpc::airmap::Traffic_Update tu;

    switch (type) {
      case airmap::Traffic::Update::Type::situational_awareness:
        tu.set_type(::grpc::airmap::Traffic_Update_Type_situational_awareness);
        break;
      case airmap::Traffic::Update::Type::alert:
        tu.set_type(::grpc::airmap::Traffic_Update_Type_alert);
        break;
      case airmap::Traffic::Update::Type::unknown:
        tu.set_type(::grpc::airmap::Traffic_Update_Type_unknown_type);
        break;
    }

    tu.set_aircraft_id(update.aircraft_id);
    tu.mutable_track()->set_as_string(update.id);
    tu.mutable_position()->mutable_latitude()->set_value(update.latitude);
    tu.mutable_position()->mutable_longitude()->set_value(update.longitude);
    tu.mutable_ground_speed()->set_value(update.ground_speed);
    tu.mutable_heading()->set_value(update.heading);
    tu.mutable_direction()->set_value(update.direction);

    auto us = microseconds_since_epoch(update.recorded);
    auto s  = us / (1000 * 1000);

    tu.mutable_recorded()->set_seconds(s);
    tu.mutable_recorded()->set_nanos(us - (s * 1000 * 1000));

    us = microseconds_since_epoch(update.timestamp);
    s  = us / (1000 * 1000);

    tu.mutable_generated()->set_seconds(s);
    tu.mutable_generated()->set_nanos(us - (s * 1000 * 1000));

    *u.add_traffic() = tu;
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