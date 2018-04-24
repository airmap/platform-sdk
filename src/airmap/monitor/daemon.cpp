#include <airmap/monitor/daemon.h>
#include <airmap/monitor/submitting_vehicle_monitor.h>

#include <airmap/monitor/grpc/service.h>

namespace {
constexpr const char* component{"airmap::monitor::Daemon"};
}  // namespace

std::shared_ptr<airmap::monitor::Daemon> airmap::monitor::Daemon::create(const Configuration& configuration) {
  return std::shared_ptr<Daemon> {
    new Daemon {
      configuration
    }
  }
  ->finalize();
}

airmap::monitor::Daemon::Daemon(const Configuration& configuration)
    : configuration_{configuration},
      log_{configuration_.logger},
      fan_out_traffic_monitor_{std::make_shared<FanOutTrafficMonitor>()},
      executor_{std::make_shared<airmap::grpc::server::Executor>(airmap::grpc::server::Executor::Configuration{
          configuration.context,
          configuration_.grpc_endpoint,
          {std::make_shared<airmap::monitor::grpc::Service>(configuration_.logger, fan_out_traffic_monitor_)},
          ::grpc::InsecureServerCredentials()})},
      executor_worker_{[this]() { executor_->run(); }} {
}

std::shared_ptr<airmap::monitor::Daemon> airmap::monitor::Daemon::finalize() {
  auto sp                  = shared_from_this();
  vehicle_tracker_monitor_ = std::make_shared<mavlink::LoggingVehicleTrackerMonitor>(component, log_.logger(), sp);

  logging_channel_subscription_ = configuration_.channel->subscribe([sp](const mavlink_message_t& msg) {
    sp->log_.debugf(component,
                    "received mavlink message:\n"
                    "  checksum: %d\n"
                    "  magic:    %d\n"
                    "  len:      %d\n"
                    "  seq:      %d\n"
                    "  sysid:    %d\n"
                    "  compid:   %d\n"
                    "  msgid:    %d",
                    msg.checksum, msg.magic, msg.len, msg.seq, msg.sysid, msg.compid, msg.msgid);
  });

  vehicle_tracker_.register_monitor(vehicle_tracker_monitor_);

  return sp;
}

airmap::monitor::Daemon::~Daemon() {
  configuration_.channel->stop();
  configuration_.channel->unsubscribe(std::move(mavlink_channel_subscription_));
  configuration_.channel->unsubscribe(std::move(logging_channel_subscription_));
  vehicle_tracker_.unregister_monitor(shared_from_this());

  executor_->stop();
  if (executor_worker_.joinable())
    executor_worker_.join();
}

void airmap::monitor::Daemon::start() {
  mavlink_channel_subscription_ = configuration_.channel->subscribe([sp = shared_from_this()](
      const mavlink_message_t& msg) { sp->handle_mavlink_message(msg); });
  configuration_.channel->start();
}

void airmap::monitor::Daemon::handle_mavlink_message(const mavlink_message_t& msg) {
  log_.debugf(component,
              "received mavlink message:\n"
              "  checksum: %d\n"
              "  magic:    %d\n"
              "  len:      %d\n"
              "  seq:      %d\n"
              "  sysid:    %d\n"
              "  compid:   %d\n"
              "  msgid:    %d",
              msg.checksum, msg.magic, msg.len, msg.seq, msg.sysid, msg.compid, msg.msgid);
  vehicle_tracker_.update(msg);
}

void airmap::monitor::Daemon::on_vehicle_added(const std::shared_ptr<mavlink::Vehicle>& vehicle) {
  auto submitter = TelemetrySubmitter::create(configuration_.credentials, configuration_.aircraft_id, log_.logger(),
                                              configuration_.client, fan_out_traffic_monitor_);
  vehicle->register_monitor(std::make_shared<mavlink::LoggingVehicleMonitor>(
      component, log_.logger(), std::make_shared<SubmittingVehicleMonitor>(submitter)));
}

void airmap::monitor::Daemon::on_vehicle_removed(const std::shared_ptr<mavlink::Vehicle>&) {
  // empty on purpose
}
