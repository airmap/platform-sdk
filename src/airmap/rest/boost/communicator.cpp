#include <airmap/rest/boost/communicator.h>

#include <airmap/rest/client.h>

#include <boost/beast/http.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <mqtt/str_connect_return_code.hpp>

#include <iostream>

namespace asio  = boost::asio;
namespace http  = boost::beast::http;
namespace ssl   = boost::asio::ssl;
namespace uuids = boost::uuids;
using tcp       = boost::asio::ip::tcp;
using udp       = boost::asio::ip::udp;

namespace {
std::exception_ptr wrap_error_code(const boost::system::error_code& ec) {
  return std::make_exception_ptr(std::runtime_error{ec.message()});
}

constexpr const char* component{"rest::boost::Communicator"};
}  // namespace

airmap::mqtt::boost::Client::Subscription::Subscription(Unsubscriber unsubscriber) : unsubscriber_{unsubscriber} {
}

airmap::mqtt::boost::Client::Subscription::~Subscription() {
  unsubscriber_();
}

std::shared_ptr<airmap::mqtt::boost::Client> airmap::mqtt::boost::Client::create(
    const std::shared_ptr<Logger>& logger, const std::shared_ptr<TlsClient>& mqtt_client) {
  return std::shared_ptr<Client>(new Client{logger, mqtt_client})->finalize();
}

airmap::mqtt::boost::Client::Client(const std::shared_ptr<Logger>& logger,
                                    const std::shared_ptr<TlsClient>& mqtt_client)
    : log_{logger}, mqtt_client_{mqtt_client} {
}

std::shared_ptr<airmap::mqtt::boost::Client> airmap::mqtt::boost::Client::finalize() {
  auto sp = shared_from_this();
  std::weak_ptr<Client> wp{sp};

  mqtt_client_->set_close_handler([wp]() {
    if (auto sp = wp.lock())
      sp->log_.infof(component, "connection to mqtt broker was closed");
  });

  mqtt_client_->set_error_handler([wp](const ::boost::system::error_code& ec) {
    if (auto sp = wp.lock())
      sp->log_.errorf(component, "failed to communicate with mqtt broker: %s", ec.message());
  });

  mqtt_client_->set_suback_handler([wp](std::uint16_t packet_id, std::vector<::boost::optional<std::uint8_t>> results) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received suback from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_puback_handler([wp](std::uint16_t packet_id) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received puback from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_pubrec_handler([wp](std::uint16_t packet_id) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received pubrec from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_pubcomp_handler([wp](std::uint16_t packet_id) {
    if (auto sp = wp.lock()) {
      sp->log_.infof(component, "received pubcomp from mqtt broker: %d", packet_id);
      return true;
    }
    return false;
  });

  mqtt_client_->set_publish_handler(
      [wp](std::uint8_t header, ::boost::optional<std::uint16_t> packet_id, std::string topic, std::string contents) {
        if (auto sp = wp.lock()) {
          sp->handle_publish(header, packet_id, topic, contents);
          return true;
        }
        return false;
      });

  return sp;
}

std::unique_ptr<airmap::mqtt::Client::Subscription> airmap::mqtt::boost::Client::subscribe(const std::string& topic,
                                                                                           QualityOfService qos,
                                                                                           PublishCallback cb) {
  auto translated = ::mqtt::qos::exactly_once;

  switch (qos) {
    case QualityOfService::at_least_once:
      translated = ::mqtt::qos::at_least_once;
      break;
    case QualityOfService::at_most_once:
      translated = ::mqtt::qos::at_most_once;
      break;
    case QualityOfService::exactly_once:
      translated = ::mqtt::qos::exactly_once;
      break;
  }

  auto id  = mqtt_client_->async_subscribe(topic, translated);
  auto itt = topic_map_.emplace(topic, cb);
  subscription_map_.emplace(id, itt);

  std::weak_ptr<Client> wp{shared_from_this()};
  std::unique_ptr<airmap::mqtt::Client::Subscription> result{new Subscription{[wp, id]() {
    if (auto sp = wp.lock()) {
      sp->unsubscribe(id);
    }
  }}};
  return result;
}

void airmap::mqtt::boost::Client::handle_publish(std::uint8_t, ::boost::optional<std::uint16_t>, std::string topic,
                                                 std::string contents) {
  log_.debugf(component, "received publish from mqtt broker for topic %s: %s", topic, contents);
  auto range = topic_map_.equal_range(topic);
  for (auto it = range.first; it != range.second; ++it) {
    it->second(topic, contents);
  }
}

void airmap::mqtt::boost::Client::unsubscribe(SubscriptionId subscription_id) {
  auto it = subscription_map_.find(subscription_id);

  if (it != subscription_map_.end()) {
    mqtt_client_->async_unsubscribe(it->second->first);
    topic_map_.erase(it->second);
    subscription_map_.erase(it);
  }
}

airmap::rest::boost::Communicator::Communicator(const std::shared_ptr<Logger>& logger)
    : log_{logger},
      io_service_{std::make_shared<asio::io_service>()},
      keep_alive_{std::make_shared<asio::io_service::work>(*io_service_)},
      state_{State::stopped},
      return_code_{Context::ReturnCode::success} {
}

airmap::rest::boost::Communicator::~Communicator() {
}

const std::shared_ptr<::boost::asio::io_service>& airmap::rest::boost::Communicator::io_service() const {
  return io_service_;
}

// From airmap::Context
void airmap::rest::boost::Communicator::create_client_with_configuration(const Client::Configuration& configuration,
                                                                         const ClientCreateCallback& cb) {
  cb(ClientCreateResult{std::make_shared<rest::Client>(configuration, log_.logger(), shared_from_this())});
}

airmap::Context::ReturnCode airmap::rest::boost::Communicator::exec(const SignalSet& signal_set,
                                                                    const SignalHandler& signal_handler) {
  asio::signal_set ss{*io_service_};

  for (auto signal : signal_set) {
    ss.add(signal);
  }

  ss.async_wait([this, signal_handler](const auto& ec, int signal) {
    if (!ec) {
      signal_handler(signal);
    } else {
      log_.errorf(component, "failed to wait for posix signals: %s", ec.message());
    }
  });

  return run();
}

airmap::Context::ReturnCode airmap::rest::boost::Communicator::run() {
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

void airmap::rest::boost::Communicator::stop(ReturnCode rc) {
  State expected{State::running};
  if (!state_.compare_exchange_strong(expected, State::stopping)) {
    return;
  }

  return_code_.store(rc);
  io_service_->stop();
}

// From airmap::rest::Communicator
void airmap::rest::boost::Communicator::connect_to_mqtt_broker(const std::string& host, std::uint16_t port,
                                                               const std::string& username, const std::string& password,
                                                               const ConnectCallback& cb) {
  auto client = ::mqtt::make_tls_client(*io_service_, host, std::to_string(port));
  client->set_clean_session(true);
  client->set_default_verify_paths();
  client->set_client_id(uuids::to_string(uuids::random_generator()()));
  client->set_user_name(username);
  client->set_password(password);

  client->set_connack_handler([ logger = log_.logger(), host, port, cb, client ](auto, auto rc) {
    if (::mqtt::connect_return_code::accepted == rc) {
      cb(ConnectResult(mqtt::boost::Client::create(logger, client)));
    } else {
      cb(ConnectResult(std::make_exception_ptr(std::runtime_error{fmt::sprintf(
          "failed to connect to mqtt broker %s:%d: %s", host, port, ::mqtt::connect_return_code_to_str(rc))})));
    }
    return ::mqtt::connect_return_code::accepted == rc;
  });

  client->connect([cb](const auto& ec) {
    if (ec) {
      cb(ConnectResult(std::make_exception_ptr(std::runtime_error{ec.message()})));
    }
  });
}
void airmap::rest::boost::Communicator::delete_(const std::string& host, const std::string& path,
                                                std::unordered_map<std::string, std::string>&& query,
                                                std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  std::make_shared<HttpSession>(HttpSession::Delete{}, log_.logger(), io_service_, host, path, std::move(query),
                                std::move(headers), std::move(cb))
      ->start();
}

void airmap::rest::boost::Communicator::get(const std::string& host, const std::string& path,
                                            std::unordered_map<std::string, std::string>&& query,
                                            std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  std::make_shared<HttpSession>(HttpSession::Get{}, log_.logger(), io_service_, host, path, std::move(query),
                                std::move(headers), std::move(cb))
      ->start();
}

void airmap::rest::boost::Communicator::patch(const std::string& host, const std::string& path,
                                              std::unordered_map<std::string, std::string>&& headers,
                                              const std::string& body, DoCallback cb) {
  std::make_shared<HttpSession>(HttpSession::Patch{}, log_.logger(), io_service_, host, path, body, std::move(headers),
                                std::move(cb))
      ->start();
}

void airmap::rest::boost::Communicator::post(const std::string& host, const std::string& path,
                                             std::unordered_map<std::string, std::string>&& headers,
                                             const std::string& body, DoCallback cb) {
  std::make_shared<HttpSession>(HttpSession::Post{}, log_.logger(), io_service_, host, path, body, std::move(headers),
                                std::move(cb))
      ->start();
}

void airmap::rest::boost::Communicator::send_udp(const std::string& host, std::uint16_t port,
                                                 const std::string& payload) {
  std::make_shared<UdpSession>(log_.logger(), io_service_, host, port, payload)->start();
}

void airmap::rest::boost::Communicator::dispatch(const std::function<void()>& task) {
  io_service_->dispatch(task);
}

airmap::rest::boost::Communicator::HttpSession::HttpSession(const Delete&, const std::shared_ptr<Logger>& logger,
                                                            const std::shared_ptr<asio::io_service>& io_service,
                                                            const std::string& host, const std::string& path,
                                                            std::unordered_map<std::string, std::string>&& query,
                                                            std::unordered_map<std::string, std::string>&& headers,
                                                            DoCallback cb)
    : log{logger},
      io_service{io_service},
      resolver{*io_service},
      ssl_context{ssl::context::sslv23},
      socket{*io_service, ssl_context},
      cb{cb} {
  ssl_context.set_default_verify_paths();
  ssl_context.set_verify_mode(ssl::verify_peer);

  network::uri_builder uri_builder;
  uri_builder.scheme("https").host(host).path(path);
  for (const auto& pair : query)
    uri_builder.append_query_key_value_pair(pair.first, pair.second);
  uri = uri_builder.uri();

  request.method(http::verb::delete_);
  request.target(fmt::sprintf("%s?%s", uri.path(), uri.query()));
  request.set(http::to_string(http::field::accept), "application/json");
  request.set(http::to_string(http::field::host), uri.host());
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();
}

airmap::rest::boost::Communicator::HttpSession::HttpSession(const Get&, const std::shared_ptr<Logger>& logger,
                                                            const std::shared_ptr<asio::io_service>& io_service,
                                                            const std::string& host, const std::string& path,
                                                            std::unordered_map<std::string, std::string>&& query,
                                                            std::unordered_map<std::string, std::string>&& headers,
                                                            DoCallback cb)
    : log{logger},
      io_service{io_service},
      resolver{*io_service},
      ssl_context{ssl::context::sslv23},
      socket{*io_service, ssl_context},
      cb{cb} {
  ssl_context.set_default_verify_paths();
  ssl_context.set_verify_mode(ssl::verify_peer);

  network::uri_builder uri_builder;
  uri_builder.scheme("https").host(host).path(path);
  for (const auto& pair : query)
    uri_builder.append_query_key_value_pair(pair.first, pair.second);
  uri = uri_builder.uri();

  request.method(http::verb::get);
  request.target(fmt::sprintf("%s?%s", uri.path(), uri.query()));
  request.set(http::to_string(http::field::accept), "application/json");
  request.set(http::to_string(http::field::host), uri.host());
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();
}

airmap::rest::boost::Communicator::HttpSession::HttpSession(const Post&, const std::shared_ptr<Logger>& logger,
                                                            const std::shared_ptr<asio::io_service>& io_service,
                                                            const std::string& host, const std::string& path,
                                                            const std::string& body,
                                                            std::unordered_map<std::string, std::string>&& headers,
                                                            DoCallback cb)
    : log{logger},
      io_service{io_service},
      resolver{*io_service},
      ssl_context{ssl::context::sslv23},
      socket{*io_service, ssl_context},
      cb{cb} {
  ssl_context.set_default_verify_paths();
  ssl_context.set_verify_mode(ssl::verify_peer);

  network::uri_builder uri_builder;
  uri = uri_builder.scheme("https").host(host).path(path).uri();

  request.method(http::verb::post);
  request.target(fmt::sprintf("%s%s", uri.path(), uri.query()));
  request.set(http::to_string(http::field::accept), "application/json");
  request.set(http::to_string(http::field::content_type), "application/json");
  request.set(http::to_string(http::field::host), uri.host());
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.body = std::move(body);
  request.prepare_payload();
}

airmap::rest::boost::Communicator::HttpSession::HttpSession(const Patch&, const std::shared_ptr<Logger>& logger,
                                                            const std::shared_ptr<asio::io_service>& io_service,
                                                            const std::string& host, const std::string& path,
                                                            const std::string& body,
                                                            std::unordered_map<std::string, std::string>&& headers,
                                                            DoCallback cb)
    : log{logger},
      io_service{io_service},
      resolver{*io_service},
      ssl_context{ssl::context::sslv23},
      socket{*io_service, ssl_context},
      cb{cb} {
  ssl_context.set_default_verify_paths();
  ssl_context.set_verify_mode(ssl::verify_peer);

  network::uri_builder uri_builder;
  uri = uri_builder.scheme("https").host(host).path(path).uri();

  request.method(http::verb::patch);
  request.target(fmt::sprintf("%s%s", uri.path(), uri.query()));
  request.set(http::to_string(http::field::accept), "application/json");
  request.set(http::to_string(http::field::content_type), "application/json");
  request.set(http::to_string(http::field::host), uri.host());
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.body = std::move(body);
  request.prepare_payload();
}

void airmap::rest::boost::Communicator::HttpSession::start() {
  resolver.async_resolve(
      tcp::resolver::query(uri.host().to_string(), uri.scheme().to_string(), tcp::resolver::query::passive),
      std::bind(&HttpSession::handle_resolve, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void airmap::rest::boost::Communicator::HttpSession::handle_resolve(const ::boost::system::error_code& error,
                                                                    tcp::resolver::iterator iterator) {
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }
  socket.lowest_layer().async_connect(
      *iterator, std::bind(&HttpSession::handle_connect, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::HttpSession::handle_connect(const ::boost::system::error_code& error) {
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }
  socket.async_handshake(ssl::stream_base::client,
                         std::bind(&HttpSession::handle_ssl_handshake, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::HttpSession::handle_ssl_handshake(const ::boost::system::error_code& error) {
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }
  http::async_write(socket, request, std::bind(&HttpSession::handle_write, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::HttpSession::handle_write(const ::boost::system::error_code& error) {
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }
  http::async_read(socket, buffer, response,
                   std::bind(&HttpSession::handle_read, shared_from_this(), std::placeholders::_1));
}

void airmap::rest::boost::Communicator::HttpSession::handle_read(const ::boost::system::error_code& error) {
  if (error) {
    cb(DoResult(wrap_error_code(error)));
    return;
  }

  auto sc = response.base().result();

  switch (http::to_status_class(sc)) {
    case http::status_class::informational:
      log.debugf(component, "received informational http response: %s", sc);
      break;
    case http::status_class::successful:
      cb(DoResult{response.body});
      break;
    case http::status_class::redirection:
      log.debugf(component, "received redirection http response: %s", sc);
      break;
    case http::status_class::client_error:
    case http::status_class::server_error:
      // TODO(tvoss): We should think about introducing an http::ClientError and
      // http::ServerError, both inheriting std::runtime_error.
      cb(DoResult{std::make_exception_ptr(std::runtime_error{fmt::sprintf("%s - %s\n%s", sc, request, response)})});
      break;
    default:
      cb(DoResult{std::make_exception_ptr(std::runtime_error{fmt::sprintf("%s\n%s", request, response)})});
      break;
  }
}

airmap::rest::boost::Communicator::UdpSession::UdpSession(const std::shared_ptr<Logger>& logger,
                                                          const std::shared_ptr<::boost::asio::io_service>& io_service,
                                                          const std::string& host, std::uint16_t port,
                                                          const std::string& payload)
    : log{logger},
      io_service{io_service},
      resolver{*io_service},
      socket{*io_service, udp::endpoint{udp::v4(), 0}},
      payload{payload} {
  network::uri_builder ub;
  ub.host(host).port(port);
  uri = ub.uri();
}

void airmap::rest::boost::Communicator::UdpSession::start() {
  resolver.async_resolve(
      udp::resolver::query(uri.host().to_string(), std::to_string(uri.port<std::uint16_t>()),
                           udp::resolver::query::passive),
      std::bind(&UdpSession::handle_resolve, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void airmap::rest::boost::Communicator::UdpSession::handle_resolve(const ::boost::system::error_code& error,
                                                                   udp::resolver::iterator iterator) {
  if (error) {
    return;
  }

  socket.async_send_to(
      ::boost::asio::buffer(payload), iterator->endpoint(),
      std::bind(&UdpSession::handle_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void airmap::rest::boost::Communicator::UdpSession::handle_write(const ::boost::system::error_code& error,
                                                                 std::size_t) {
  if (error) {
    log.errorf(component, "failed to send udp data to %s:%d: %s", uri.host().to_string(), uri.port<std::uint16_t>(),
               error.message());
  }
}
