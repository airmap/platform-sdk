//
//  request.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/net/http/boost/request.h>

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace ssl  = boost::asio::ssl;

namespace {

airmap::Error wrap_error_code(const boost::system::error_code& ec) {
  return airmap::Error{ec.message()};
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

  cb_(Result{Response{response_.version(), response_.result_int(), {}, response_.body()}});
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

  cb_(Result{Response{response_.version(), response_.result_int(), {}, response_.body()}});
}
