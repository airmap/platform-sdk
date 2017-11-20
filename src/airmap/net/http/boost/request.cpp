#include <airmap/net/http/boost/request.h>

#include <fmt/printf.h>
#include <boost/lexical_cast.hpp>

#include <stdexcept>

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace ssl  = boost::asio::ssl;

namespace {

airmap::Error wrap_error_code(const boost::system::error_code& ec) {
  airmap::Error error;
  error.message = ec.message();
  return error;
}

constexpr const char* component{"airmap::net::http::boost::Request"};

}  // namespace

std::shared_ptr<airmap::net::http::boost::NonEncryptingRequest> airmap::net::http::boost::NonEncryptingRequest::create(
    const Configuration& configuration) {
  return std::shared_ptr<NonEncryptingRequest>{new NonEncryptingRequest{configuration}};
}

airmap::net::http::boost::NonEncryptingRequest::NonEncryptingRequest(const Configuration& configuration)
    : log_{configuration.logger},
      io_service_{configuration.io_service},
      endpoint_{std::move(configuration.endpoint)},
      socket_{*io_service_},
      request_{std::move(configuration.request)},
      cb_{std::move(configuration.cb)} {
}

void airmap::net::http::boost::NonEncryptingRequest::start() {
  socket_.async_connect(endpoint_,
                        std::bind(&NonEncryptingRequest::handle_connect, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::NonEncryptingRequest::handle_connect(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  ::http::async_write(socket_, request_,
                      std::bind(&NonEncryptingRequest::handle_write, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::NonEncryptingRequest::handle_write(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  ::http::async_read(socket_, buffer_, response_,
                     std::bind(&NonEncryptingRequest::handle_read, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::NonEncryptingRequest::handle_read(const ::boost::system::error_code& error) {
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
    case ::http::status_class::client_error: {
      Error err;
      err.message               = "client error";
      err.values["status-code"] = Error::Value{static_cast<std::int64_t>(sc)};
      err.values["request"]     = Error::Value{std::string{::boost::lexical_cast<std::string>(request_)}};
      err.values["response"]    = Error::Value{std::string{::boost::lexical_cast<std::string>(response_)}};
      cb_(Result{err});
      break;
    }
    case ::http::status_class::server_error: {
      Error err;
      err.message               = "server error";
      err.values["status-code"] = Error::Value{static_cast<std::int64_t>(sc)};
      err.values["request"]     = Error::Value{std::string{::boost::lexical_cast<std::string>(request_)}};
      err.values["response"]    = Error::Value{std::string{::boost::lexical_cast<std::string>(response_)}};
      cb_(Result{err});
      break;
    }
    default: {
      Error err;
      err.message               = "networking error";
      err.values["status-code"] = Error::Value{static_cast<std::int64_t>(sc)};
      err.values["request"]     = Error::Value{std::string{::boost::lexical_cast<std::string>(request_)}};
      err.values["response"]    = Error::Value{std::string{::boost::lexical_cast<std::string>(response_)}};
      cb_(Result{err});
      break;
    }
  }
}

std::shared_ptr<airmap::net::http::boost::EncryptingRequest> airmap::net::http::boost::EncryptingRequest::create(
    const Configuration& configuration) {
  return std::shared_ptr<EncryptingRequest>{new EncryptingRequest{configuration}};
}

airmap::net::http::boost::EncryptingRequest::EncryptingRequest(const Configuration& configuration)
    : log_{configuration.logger},
      io_service_{configuration.io_service},
      endpoint_{std::move(configuration.endpoint)},
      ssl_context_{ssl::context::sslv23},
      socket_{*io_service_, ssl_context_},
      request_{std::move(configuration.request)},
      cb_{std::move(configuration.cb)} {
  ssl_context_.set_default_verify_paths();
  ssl_context_.set_verify_mode(ssl::verify_peer);
}

void airmap::net::http::boost::EncryptingRequest::start() {
  socket_.lowest_layer().async_connect(
      endpoint_, std::bind(&EncryptingRequest::handle_connect, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::EncryptingRequest::handle_connect(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  socket_.async_handshake(ssl::stream_base::client, std::bind(&EncryptingRequest::handle_ssl_handshake,
                                                              shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::EncryptingRequest::handle_ssl_handshake(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  ::http::async_write(socket_, request_,
                      std::bind(&EncryptingRequest::handle_write, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::EncryptingRequest::handle_write(const ::boost::system::error_code& error) {
  if (error) {
    cb_(Result(wrap_error_code(error)));
    return;
  }
  ::http::async_read(socket_, buffer_, response_,
                     std::bind(&EncryptingRequest::handle_read, shared_from_this(), std::placeholders::_1));
}

void airmap::net::http::boost::EncryptingRequest::handle_read(const ::boost::system::error_code& error) {
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
    case ::http::status_class::client_error: {
      Error err;
      err.message               = "client error";
      err.values["status-code"] = Error::Value{static_cast<std::int64_t>(sc)};
      err.values["request"]     = Error::Value{std::string{::boost::lexical_cast<std::string>(request_)}};
      err.values["response"]    = Error::Value{std::string{::boost::lexical_cast<std::string>(response_)}};
      cb_(Result{err});
      break;
    }
    case ::http::status_class::server_error: {
      Error err;
      err.message               = "server error";
      err.values["status-code"] = Error::Value{static_cast<std::int64_t>(sc)};
      err.values["request"]     = Error::Value{std::string{::boost::lexical_cast<std::string>(request_)}};
      err.values["response"]    = Error::Value{std::string{::boost::lexical_cast<std::string>(response_)}};
      cb_(Result{err});
      break;
    }
    default: {
      Error err;
      err.message               = "networking error";
      err.values["status-code"] = Error::Value{static_cast<std::int64_t>(sc)};
      err.values["request"]     = Error::Value{std::string{::boost::lexical_cast<std::string>(request_)}};
      err.values["response"]    = Error::Value{std::string{::boost::lexical_cast<std::string>(response_)}};
      cb_(Result{err});
      break;
    }
  }
}
