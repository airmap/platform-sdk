#ifndef AIRMAP_NET_HTTP_BOOST_REQUESTER_H_
#define AIRMAP_NET_HTTP_BOOST_REQUESTER_H_

#include <airmap/net/http/requester.h>

#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

namespace airmap {
namespace net {
namespace http {
namespace boost {

class Requester : public http::Requester, public std::enable_shared_from_this<Requester> {
 public:
  static std::shared_ptr<Requester> create(const std::string& host, std::uint16_t port,
                                           const std::shared_ptr<Logger>& logger,
                                           const std::shared_ptr<::boost::asio::io_service>& io_service);

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
                     const std::shared_ptr<::boost::asio::io_service>& io_service);
  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::tcp::resolver resolver_;
  std::string host_;
  std::uint16_t port_;
};

}  // namespace boost
}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_BOOST_REQUESTER_H_
