//
//  udp_channel.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/mavlink/boost/udp_channel.h>

namespace {
constexpr const char* component{"airmap::mavlink::UdpChannel"};
}  // namespace

airmap::mavlink::boost::UdpChannel::UdpChannel(const std::shared_ptr<Logger>& logger,
                                               const std::shared_ptr<::boost::asio::io_service>& io_service,
                                               std::uint16_t port)
    : log_{logger},
      io_service_{io_service},
      socket_{*io_service_, ::boost::asio::ip::udp::endpoint{::boost::asio::ip::udp::v4(), port}} {
}

void airmap::mavlink::boost::UdpChannel::start_impl() {
  using std::placeholders::_1;
  using std::placeholders::_2;

  ::boost::asio::ip::udp::endpoint endpoint;
  socket_.async_receive_from(::boost::asio::buffer(buffer_), endpoint,
                             std::bind(&UdpChannel::handle_read, shared_from_this(), _1, _2));
}

void airmap::mavlink::boost::UdpChannel::stop_impl() {
  socket_.cancel();
}

void airmap::mavlink::boost::UdpChannel::handle_read(const ::boost::system::error_code& ec, std::size_t transferred) {
  if (ec) {
    log_.errorf(component, "failed to read from tcp endpoint: %s", ec.message());
    return;
  }

  if (auto result = process_mavlink_data(buffer_.begin(), buffer_.begin() + transferred))
    invoke_subscribers(result.get());

  start_impl();
}