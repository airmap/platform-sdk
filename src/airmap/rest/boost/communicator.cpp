#include <airmap/rest/boost/communicator.h>

#include <airmap/rest/client.h>

#include <boost/beast/http.hpp>

#include <iostream>

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace ssl  = boost::asio::ssl;
using tcp      = boost::asio::ip::tcp;
using udp      = boost::asio::ip::udp;

namespace {
std::exception_ptr wrap_error_code(const boost::system::error_code& ec) {
  return std::make_exception_ptr(std::runtime_error{ec.message()});
}

constexpr const char* component{"rest::boost::Communicator"};
}  // namespace

airmap::rest::boost::Communicator::Communicator(const std::shared_ptr<Logger>& logger)
    : log_{logger},
      io_service_{std::make_shared<asio::io_service>()},
      keep_alive_{std::make_shared<asio::io_service::work>(*io_service_)} {
}

airmap::rest::boost::Communicator::~Communicator() {
}

// From airmap::Context
void airmap::rest::boost::Communicator::create_client_with_credentials(const Client::Credentials& credentials,
                                                                       const ClientCreateCallback& cb) {
  cb(ClientCreateResult{std::make_shared<rest::Client>(credentials, shared_from_this())});
}

void airmap::rest::boost::Communicator::run() {
  while (!io_service_->stopped()) {
    try {
      io_service_->run();
    } catch (const std::exception& e) {
      log_.errorf(component, "error while running the context: %s", e.what());
    } catch (...) {
      log_.errorf(component, "error while running the context");
    }
  }
}

void airmap::rest::boost::Communicator::stop() {
  io_service_->stop();
}

// From airmap::rest::Communicator
void airmap::rest::boost::Communicator::get(const std::string& host, const std::string& path,
                                            std::unordered_map<std::string, std::string>&& query,
                                            std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  std::make_shared<HttpSession>(HttpSession::Get{}, log_.logger(), io_service_, host, path, std::move(query),
                                std::move(headers), std::move(cb))
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

airmap::rest::boost::Communicator::HttpSession::HttpSession(const Get&, const std::shared_ptr<Logger>& logger,
                                                            const std::shared_ptr<asio::io_service>& io_service,
                                                            const std::string& host, const std::string& path,
                                                            std::unordered_map<std::string, std::string>&& query,
                                                            std::unordered_map<std::string, std::string>&& headers,
                                                            DoCallback cb)
    : log{logger},
      io_service{io_service},
      ssl_context{ssl::context::sslv23},
      resolver{*io_service},
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
  request.target(fmt::sprintf("%s/%s", uri.path(), uri.query()));
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
      ssl_context{ssl::context::sslv23},
      resolver{*io_service},
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

  switch (response.base().result()) {
    case http::status::ok:
      cb(DoResult{response.body});
      break;
    default:
      cb(DoResult{std::make_exception_ptr(std::runtime_error{fmt::sprintf("%s", request)})});
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
                                                                 std::size_t transferred) {
  if (error) {
    log.errorf(component, "failed to send udp data to %s:%d: %s", uri.host().to_string(), uri.port<std::uint16_t>(),
               error.message());
  }
  log.infof(component, "sent %d bytes to %s:%d", transferred, uri.host().to_string(), uri.port());
}