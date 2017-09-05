#include <airmap/boost/context.h>

#include <airmap/net/http/boost/requester.h>
#include <airmap/net/mqtt/boost/broker.h>
#include <airmap/net/udp/boost/sender.h>

#include <airmap/rest/client.h>

namespace {

constexpr const char* component{"airmap::boost::Context"};

}  // namespace

std::shared_ptr<airmap::boost::Context> airmap::boost::Context::create(const std::shared_ptr<Logger>& logger) {
  return std::shared_ptr<Context>{new Context{logger}};
}

airmap::boost::Context::Context(const std::shared_ptr<Logger>& logger)
    : log_{logger},
      io_service_{std::make_shared<::boost::asio::io_service>()},
      keep_alive_{std::make_shared<::boost::asio::io_service::work>(*io_service_)},
      state_{State::stopped},
      return_code_{Context::ReturnCode::success} {
}

airmap::boost::Context::~Context() {
}

const std::shared_ptr<::boost::asio::io_service>& airmap::boost::Context::io_service() const {
  return io_service_;
}

// From airmap::Context
void airmap::boost::Context::create_client_with_configuration(const Client::Configuration& configuration,
                                                              const ClientCreateCallback& cb) {
  auto sp = shared_from_this();
  auto udp_sender =
      net::udp::boost::Sender::create(configuration.telemetry.host, configuration.telemetry.port, io_service_);
  auto airmap_http_requester = net::http::boost::Requester::create(configuration.host, 443, log_.logger(), io_service_);
  auto sso_http_requester =
      net::http::boost::Requester::create(configuration.sso.host, configuration.sso.port, log_.logger(), io_service_);
  auto mqtt_broker = std::make_shared<net::mqtt::boost::Broker>(configuration.traffic.host, configuration.traffic.port,
                                                                log_.logger(), io_service_);
  cb(ClientCreateResult{std::make_shared<rest::Client>(configuration, sp, udp_sender, airmap_http_requester,
                                                       sso_http_requester, mqtt_broker)});
}

airmap::Context::ReturnCode airmap::boost::Context::exec(const SignalSet& signal_set,
                                                         const SignalHandler& signal_handler) {
  auto ss = std::make_shared<::boost::asio::signal_set>(*io_service_);

  for (auto signal : signal_set)
    ss->add(signal);

  ss->async_wait([this, signal_handler](const auto& ec, int signal) {
    if (!ec) {
      signal_handler(signal);
    } else {
      log_.errorf(component, "failed to wait for posix signals: %s", ec.message());
    }
  });

  return run();
}

airmap::Context::ReturnCode airmap::boost::Context::run() {
  State expected{State::stopped};
  if (!state_.compare_exchange_strong(expected, State::running)) {
    return Context::ReturnCode::already_running;
  }

  return_code_.store(Context::ReturnCode::success);

  while (!io_service_->stopped()) {
    try {
      io_service_->run();
    } catch (const std::exception& e) {
      log_.errorf(component, "error while running the context: %s", e.what());
    } catch (...) {
      log_.errorf(component, "error while running the context");
    }
  }

  state_.store(State::stopped);
  return return_code_.load();
}

void airmap::boost::Context::stop(ReturnCode rc) {
  State expected{State::running};
  if (!state_.compare_exchange_strong(expected, State::stopping)) {
    return;
  }

  return_code_.store(rc);
  io_service_->stop();
}

void airmap::boost::Context::dispatch(const std::function<void()>& task) {
  io_service_->dispatch(task);
}