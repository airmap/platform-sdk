#include <airmap/boost/context.h>

#include <airmap/monitor/grpc/client.h>

#include <airmap/net/http/boost/requester.h>
#include <airmap/net/mqtt/boost/broker.h>
#include <airmap/net/udp/boost/sender.h>

#include <airmap/rest/client.h>

#include <cstdlib>

namespace {

constexpr const char* component{"airmap::boost::Context"};

namespace env {

std::string get(const char* name, const std::string& default_value) {
  if (auto var = ::getenv(name))
    return var;
  return default_value;
}

}  // namespace env
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

  rest::Client::Requesters requesters;
  requesters.advisory      = advisory(configuration);
  requesters.aircrafts     = aircrafts(configuration);
  requesters.airspaces     = airspaces(configuration);
  requesters.authenticator = authenticator(configuration);
  requesters.flight_plans  = flight_plans(configuration);
  requesters.flights       = flights(configuration);
  requesters.pilots        = pilots(configuration);
  requesters.rulesets      = rulesets(configuration);
  requesters.status        = status(configuration);
  requesters.sso           = sso(configuration);

  auto mqtt_broker = std::make_shared<net::mqtt::boost::Broker>(configuration.traffic.host, configuration.traffic.port,
                                                                log_.logger(), io_service_);
  cb(ClientCreateResult{std::make_shared<rest::Client>(configuration, sp, udp_sender, requesters, mqtt_broker)});
}

void airmap::boost::Context::create_monitor_client_with_configuration(
    const monitor::Client::Configuration& configuration, const MonitorClientCreateCallback& cb) {
  auto sp     = shared_from_this();
  auto client = std::make_shared<monitor::grpc::Client>(configuration, sp);

  dispatch([cb, client]() { cb(MonitorClientCreateResult{client}); });
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

void airmap::boost::Context::schedule_in(const Microseconds& wait_for, const std::function<void()>& functor) {
  auto timer = std::make_shared<::boost::asio::deadline_timer>(*io_service_);
  timer->expires_from_now(wait_for);
  timer->async_wait([this, timer, functor](const auto& error) {
    if (error) {
      log_.errorf(component, "error waiting for timer: %s", error.message());
    } else {
      functor();
    }
  });
}

void airmap::boost::Context::dispatch(const std::function<void()>& task) {
  io_service_->post(task);
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::advisory(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_ADVISORY", "https");
  auto host     = env::get("AIRMAP_HOST_ADVISORY", configuration.host);
  auto port     = env::get("AIRMAP_PORT_ADVISORY", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_ADVISORY", rest::Advisory::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::aircrafts(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_AIRCRAFTS", "https");
  auto host     = env::get("AIRMAP_HOST_AIRCRAFTS", configuration.host);
  auto port     = env::get("AIRMAP_PORT_AIRCRAFTS", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_AIRCRAFTS", rest::Aircrafts::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::airspaces(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_AIRSPACES", "https");
  auto host     = env::get("AIRMAP_HOST_AIRSPACES", configuration.host);
  auto port     = env::get("AIRMAP_PORT_AIRSPACES", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_AIRSPACES", rest::Airspaces::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::authenticator(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_AUTHENTICATOR", "https");
  auto host     = env::get("AIRMAP_HOST_AUTHENTICATOR", configuration.host);
  auto port     = env::get("AIRMAP_PORT_AUTHENTICATOR", ::boost::lexical_cast<std::string>(443));
  auto route =
      env::get("AIRMAP_ROUTE_AUTHENTICATOR", rest::Authenticator::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::flights(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_FLIGHTS", "https");
  auto host     = env::get("AIRMAP_HOST_FLIGHTS", configuration.host);
  auto port     = env::get("AIRMAP_PORT_FLIGHTS", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_FLIGHTS", rest::Flights::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::flight_plans(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_FLIGHTS", "https");
  auto host     = env::get("AIRMAP_HOST_FLIGHTS", configuration.host);
  auto port     = env::get("AIRMAP_PORT_FLIGHTS", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_FLIGHTS", rest::FlightPlans::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::pilots(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_PILOTS", "https");
  auto host     = env::get("AIRMAP_HOST_PILOTS", configuration.host);
  auto port     = env::get("AIRMAP_PORT_PILOTS", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_PILOTS", rest::Pilots::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::rulesets(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_RULESETS", "https");
  auto host     = env::get("AIRMAP_HOST_RULESETS", configuration.host);
  auto port     = env::get("AIRMAP_PORT_RULESETS", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_RULESETS", rest::RuleSets::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::status(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_STATUS", "https");
  auto host     = env::get("AIRMAP_HOST_STATUS", configuration.host);
  auto port     = env::get("AIRMAP_PORT_STATUS", ::boost::lexical_cast<std::string>(443));
  auto route    = env::get("AIRMAP_ROUTE_STATUS", rest::Status::default_route_for_version(configuration.version));
  return std::make_shared<net::http::RoutingRequester>(
      route, std::make_shared<net::http::LoggingRequester>(
                 log_.logger(), net::http::boost::Requester::create(
                                    host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                    net::http::boost::Requester::request_factory_for_protocol(protocol))));
}

std::shared_ptr<airmap::net::http::Requester> airmap::boost::Context::sso(
    const airmap::Client::Configuration& configuration) {
  auto protocol = env::get("AIRMAP_PROTOCOL_SSO", "https");
  auto host     = env::get("AIRMAP_HOST_SSO", configuration.sso.host);
  auto port     = env::get("AIRMAP_PORT_SSO", ::boost::lexical_cast<std::string>(configuration.sso.port));

  return std::make_shared<net::http::LoggingRequester>(
      log_.logger(),
      net::http::boost::Requester::create(host, ::boost::lexical_cast<std::uint16_t>(port), log_.logger(), io_service_,
                                          net::http::boost::Requester::request_factory_for_protocol(protocol)));
}
