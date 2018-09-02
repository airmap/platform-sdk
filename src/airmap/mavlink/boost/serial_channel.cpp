//
//  serial_channel.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/mavlink/boost/serial_channel.h>

#include <termios.h>

namespace {
constexpr const char* component{"airmap::mavlink::SerialChannel"};
}  // namespace

airmap::mavlink::boost::SerialChannel::SerialChannel(const std::shared_ptr<Logger>& logger,
                                                     const std::shared_ptr<::boost::asio::io_service>& io_service,
                                                     const std::string& device_file)
    : log_{logger}, io_service_{io_service}, serial_port_{*io_service_, device_file} {
  serial_port_.set_option(::boost::asio::serial_port::baud_rate(57600));
}

void airmap::mavlink::boost::SerialChannel::start_impl() {
  flush();
  start_read();
}

void airmap::mavlink::boost::SerialChannel::stop_impl() {
  serial_port_.cancel();
}

void airmap::mavlink::boost::SerialChannel::flush() {
  ::tcflush(serial_port_.native_handle(), TCIOFLUSH);
}

void airmap::mavlink::boost::SerialChannel::start_read() {
  using std::placeholders::_1;
  using std::placeholders::_2;
  serial_port_.async_read_some(::boost::asio::buffer(buffer_),
                               std::bind(&SerialChannel::handle_read, shared_from_this(), _1, _2));
}

void airmap::mavlink::boost::SerialChannel::handle_read(const ::boost::system::error_code& ec,
                                                        std::size_t transferred) {
  if (ec) {
    log_.errorf(component, "failed to read from serial device: %s", ec.message());
    return;
  }

  if (auto result = process_mavlink_data(buffer_.begin(), buffer_.begin() + transferred)) {
    invoke_subscribers(result.get());
  }

  start_read();
}