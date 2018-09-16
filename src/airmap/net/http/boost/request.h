/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  request.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_NET_HTTP_BOOST_REQUEST_H_
#define AIRMAP_NET_HTTP_BOOST_REQUEST_H_

#include <airmap/net/http/requester.h>

#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <functional>
#include <stdexcept>
#include <string>

namespace airmap {
namespace net {
namespace http {
namespace boost {

class Request : DoNotCopyOrMove {
 public:
  struct Configuration {
    std::shared_ptr<Logger> logger;
    std::shared_ptr<::boost::asio::io_service> io_service;
    ::boost::asio::ip::tcp::endpoint endpoint;
    ::boost::beast::http::request<::boost::beast::http::string_body> request;
    Requester::Callback cb;
  };

  virtual void start() = 0;

 protected:
  Request() = default;
};

class NonEncryptingRequest : public Request, public std::enable_shared_from_this<NonEncryptingRequest> {
 public:
  using Result   = Requester::Result;
  using Callback = Requester::Callback;

  static std::shared_ptr<NonEncryptingRequest> create(const Configuration& configuration);

  void start() override;

 private:
  explicit NonEncryptingRequest(const Configuration& configuration);

  void handle_connect(const ::boost::system::error_code& error);
  void handle_write(const ::boost::system::error_code& error);
  void handle_read(const ::boost::system::error_code& error);

  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::tcp::endpoint endpoint_;
  ::boost::asio::ip::tcp::socket socket_;
  ::boost::beast::http::request<::boost::beast::http::string_body> request_;
  ::boost::beast::http::response<::boost::beast::http::string_body> response_;
  ::boost::beast::flat_buffer buffer_{8192};
  Requester::Callback cb_;
};

class EncryptingRequest : public Request, public std::enable_shared_from_this<EncryptingRequest> {
 public:
  using Result   = Requester::Result;
  using Callback = Requester::Callback;

  static std::shared_ptr<EncryptingRequest> create(const Configuration& configuration);

  void start() override;

 private:
  explicit EncryptingRequest(const Configuration& configuration);

  void handle_connect(const ::boost::system::error_code& error);
  void handle_ssl_handshake(const ::boost::system::error_code& error);
  void handle_write(const ::boost::system::error_code& error);
  void handle_read(const ::boost::system::error_code& error);

  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::tcp::endpoint endpoint_;
  ::boost::asio::ssl::context ssl_context_;
  ::boost::asio::ssl::stream<::boost::asio::ip::tcp::socket> socket_;
  ::boost::beast::http::request<::boost::beast::http::string_body> request_;
  ::boost::beast::http::response<::boost::beast::http::string_body> response_;
  ::boost::beast::flat_buffer buffer_{8192};
  Requester::Callback cb_;
};

}  // namespace boost
}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_REQUEST_H_
