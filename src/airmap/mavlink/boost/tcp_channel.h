//
//  tcp_channel.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MAVLINK_BOOST_TCP_CHANNEL_H_
#define AIRMAP_MAVLINK_BOOST_TCP_CHANNEL_H_

#include <airmap/logger.h>
#include <airmap/mavlink/channel.h>
#include <airmap/optional.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

namespace airmap {
namespace mavlink {
namespace boost {

class TcpChannel : public Channel, public std::enable_shared_from_this<TcpChannel> {
 public:
  static constexpr std::size_t buffer_size{1024};

  explicit TcpChannel(const std::shared_ptr<Logger>& logger,
                      const std::shared_ptr<::boost::asio::io_service>& io_service,
                      const ::boost::asio::ip::address& ip, std::uint16_t port);

 protected:
  // From Channel
  void start_impl() override;
  void stop_impl() override;

 private:
  void handle_read(const ::boost::system::error_code& ec, std::size_t transferred);

  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::tcp::endpoint endpoint_;
  ::boost::asio::ip::tcp::socket socket_;
  std::array<char, buffer_size> buffer_;
};

}  // namespace boost
}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_BOOST_TCP_CHANNEL_H_