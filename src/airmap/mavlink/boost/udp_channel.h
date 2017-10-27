#ifndef AIRMAP_MAVLINK_BOOST_UDP_CHANNEL_H_
#define AIRMAP_MAVLINK_BOOST_UDP_CHANNEL_H_

#include <airmap/logger.h>
#include <airmap/mavlink/channel.h>
#include <airmap/optional.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

namespace airmap {
namespace mavlink {
namespace boost {

class UdpChannel : public Channel, public std::enable_shared_from_this<UdpChannel> {
 public:
  static constexpr std::size_t buffer_size{1024};

  explicit UdpChannel(const std::shared_ptr<Logger>& logger,
                      const std::shared_ptr<::boost::asio::io_service>& io_service, std::uint16_t port);

 protected:
  // From Channel
  void start_impl() override;
  void stop_impl() override;

 private:
  void handle_read(const ::boost::system::error_code& ec, std::size_t transferred);

  util::FormattingLogger log_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::udp::socket socket_;
  std::array<char, buffer_size> buffer_;
};

}  // namespace boost
}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_BOOST_UDP_CHANNEL_H_