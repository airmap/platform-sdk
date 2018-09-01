//
//  requester.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_NET_HTTP_BOOST_REQUESTER_H_
#define AIRMAP_NET_HTTP_BOOST_REQUESTER_H_

#include <airmap/net/http/requester.h>

#include <airmap/net/http/boost/request.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

namespace airmap {
namespace net {
namespace http {
namespace boost {

class Requester : public http::Requester, public std::enable_shared_from_this<Requester> {
 public:
  using RequestFactory = std::function<std::shared_ptr<Request>(const Request::Configuration&)>;

  static RequestFactory request_factory_for_protocol(const std::string& protocol);
  static RequestFactory encrypting_request_factory();
  static RequestFactory non_encrypting_request_factory();

  static std::shared_ptr<Requester> create(const std::string& host, std::uint16_t port,
                                           const std::shared_ptr<Logger>& logger,
                                           const std::shared_ptr<::boost::asio::io_service>& io_service,
                                           const RequestFactory& request_factory);

  void delete_(const std::string& path, std::unordered_map<std::string, std::string>&& query,
               std::unordered_map<std::string, std::string>&& headers, Callback cb) override;
  void get(const std::string& path, std::unordered_map<std::string, std::string>&& query,
           std::unordered_map<std::string, std::string>&& headers, Callback cb) override;
  void patch(const std::string& path, std::unordered_map<std::string, std::string>&& headers, const std::string& body,
             Callback cb) override;
  void post(const std::string& path, std::unordered_map<std::string, std::string>&& headers, const std::string& body,
            Callback cb) override;

 private:
  explicit Requester(const std::string& host, std::uint16_t port, const std::shared_ptr<Logger>& logger,
                     const std::shared_ptr<::boost::asio::io_service>& io_service,
                     const RequestFactory& request_factory);
  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::tcp::resolver resolver_;
  std::string host_;
  std::uint16_t port_;
  RequestFactory request_factory_;
};

}  // namespace boost
}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_BOOST_REQUESTER_H_
