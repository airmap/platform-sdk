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
#include <airmap/mavlink/boost/tcp_channel.h>

namespace {
constexpr const char* component{"airmap::mavlink::TcpChannel"};
}  // namespace

airmap::mavlink::boost::TcpChannel::TcpChannel(const std::shared_ptr<Logger>& logger,
                                               const std::shared_ptr<::boost::asio::io_service>& io_service,
                                               const ::boost::asio::ip::address& ip, std::uint16_t port)
    : log_{logger}, io_service_{io_service}, endpoint_{ip, port}, socket_{*io_service_} {
  socket_.connect(endpoint_);
}

void airmap::mavlink::boost::TcpChannel::start_impl() {
  using std::placeholders::_1;
  using std::placeholders::_2;
  socket_.async_read_some(::boost::asio::buffer(buffer_),
                          std::bind(&TcpChannel::handle_read, shared_from_this(), _1, _2));
}

void airmap::mavlink::boost::TcpChannel::stop_impl() {
  socket_.cancel();
}

void airmap::mavlink::boost::TcpChannel::handle_read(const ::boost::system::error_code& ec, std::size_t transferred) {
  if (ec) {
    log_.errorf(component, "failed to read from tcp endpoint: %s", ec.message());
    return;
  }

  if (auto result = process_mavlink_data(buffer_.begin(), buffer_.begin() + transferred)) {
    log_.debugf(component, "handing %d message to subscribers", result.get().size());
    invoke_subscribers(result.get());
  }

  start_impl();
}
