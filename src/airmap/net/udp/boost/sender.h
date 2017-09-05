#ifndef AIRMAP_NET_UDP_BOOST_SENDER_H_
#define AIRMAP_NET_UDP_BOOST_SENDER_H_

#include <airmap/net/udp/sender.h>

#include <boost/asio.hpp>

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

namespace airmap {
namespace net {
namespace udp {
namespace boost {

class Sender : public udp::Sender, public std::enable_shared_from_this<Sender> {
 public:
  static std::shared_ptr<Sender> create(const std::string& host, std::uint16_t port,
                                        const std::shared_ptr<::boost::asio::io_service>& io_service);

  // From udp::Sender
  void send(const std::string& message, const Callback& cb) override;

 private:
  class Session : public std::enable_shared_from_this<Session> {
   public:
    static std::shared_ptr<Session> create(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                           const ::boost::asio::ip::udp::endpoint& endpoint, const std::string& message,
                                           const Callback& cb);

    void start();

   private:
    explicit Session(const std::shared_ptr<::boost::asio::io_service>& io_service,
                     const ::boost::asio::ip::udp::endpoint& endpoint, const std::string& message, const Callback& cb);

    void handle_write(const ::boost::system::error_code& error, std::size_t transferred);

    std::shared_ptr<::boost::asio::io_service> io_service_;
    ::boost::asio::ip::udp::endpoint endpoint_;
    ::boost::asio::ip::udp::socket socket_;
    std::string message_;
    Callback cb_;
  };

  explicit Sender(const std::string& host, std::uint16_t port,
                  const std::shared_ptr<::boost::asio::io_service>& io_service);

  std::string host_;
  std::uint16_t port_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::udp::resolver resolver_;
};

}  // namespace boost
}  // namespace udp
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_UDP_BOOST_SENDER_H_