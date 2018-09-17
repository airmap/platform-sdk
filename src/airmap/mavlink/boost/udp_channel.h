// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
