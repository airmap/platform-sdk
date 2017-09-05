#include <airmap/net/http/boost/request.h>

#include <fmt/printf.h>

#include <stdexcept>

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace ssl  = boost::asio::ssl;

namespace {

std::exception_ptr wrap_error_code(const boost::system::error_code& ec) {
  return std::make_exception_ptr(std::runtime_error{ec.message()});
}

constexpr const char* component{"airmap::net::http::boost::Request"};

}  // namespace

std::shared_ptr<airmap::net::http::boost::Request> airmap::net::http::boost::Request::create(
    const std::shared_ptr<Logger>& logger, const std::shared_ptr<::boost::asio::io_service>& io_service,
    ::boost::asio::ip::tcp::endpoint endpoint, ::boost::beast::http::request<::boost::beast::http::string_body> request,
    Callback cb) {
  return std::shared_ptr<Request>{new Request{logger, io_service, endpoint, request, std::move(cb)}};
}

airmap::net::http::boost::Request::Request(const std::shared_ptr<Logger>& logger,
                                           const std::shared_ptr<::boost::asio::io_service>& io_service,
                                           ::boost::asio::ip::tcp::endpoint endpoint,
                                           ::boost::beast::http::request<::boost::beast::http::string_body> request,
                                           Callback cb)
    : log_{logger},
      io_service_{io_service},
      endpoint_{std::move(endpoint)},
      ssl_context_{ssl::context::sslv23},
      socket_{*io_service_, ssl_context_},
      request_{std::move(request)},
      cb_{std::move(cb)} {
  ssl_context_.set_default_verify_paths();
  ssl_context_.set_verify_mode(ssl::verify_peer);
}

void airmap::net::http::boost::Request::start() {
  socket_.lowest_layer().async_connect(endpoint_,
                                       std::bind(&Request::handle_connect, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::Request::handle_connect(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  socket_.async_handshake(ssl::stream_base::client,
                          std::bind(&Request::handle_ssl_handshake, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::Request::handle_ssl_handshake(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  ::http::async_write(socket_, request_, std::bind(&Request::handle_write, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::Request::handle_write(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  ::http::async_read(socket_, buffer_, response_,
                     std::bind(&Request::handle_read, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::Request::handle_read(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }

  auto sc = response_.base().result();

  switch (::http::to_status_class(sc)) {
    case ::http::status_class::informational:
      log_.debugf(component, "received informational http response: %s", sc);
      break;
    case ::http::status_class::successful:
      cb_(Result{Response{response_.version, response_.result_int(), {}, response_.body}});
      break;
    case ::http::status_class::redirection:
      log_.debugf(component, "received redirection http response: %s", sc);
      break;
    case ::http::status_class::client_error:
    case ::http::status_class::server_error:
      // TODO(tvoss): We should think about introducing an http::ClientError and
      // http::ServerError, both inheriting std::runtime_error.
      cb_(Result{std::make_exception_ptr(std::runtime_error{fmt::sprintf("%s - %s\n%s", sc, request_, response_)})});
      break;
    default:
      cb_(Result{std::make_exception_ptr(std::runtime_error{fmt::sprintf("%s\n%s", request_, response_)})});
      break;
  }
}
