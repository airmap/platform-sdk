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

class Request : public std::enable_shared_from_this<Request> {
 public:
  using Result   = Requester::Result;
  using Callback = Requester::Callback;

  static std::shared_ptr<Request> create(const std::shared_ptr<Logger>& logger,
                                         const std::shared_ptr<::boost::asio::io_service>& io_service,
                                         ::boost::asio::ip::tcp::endpoint endpoint,
                                         ::boost::beast::http::request<::boost::beast::http::string_body> request,
                                         Callback cb);

  void start();

 private:
  explicit Request(const std::shared_ptr<Logger>& logger, const std::shared_ptr<::boost::asio::io_service>& io_service,
                   ::boost::asio::ip::tcp::endpoint endpoint,
                   ::boost::beast::http::request<::boost::beast::http::string_body> request, Callback cb);

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