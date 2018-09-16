/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  tcp_route.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MAVLINK_BOOST_TCP_ROUTE_H_
#define AIRMAP_MAVLINK_BOOST_TCP_ROUTE_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/mavlink/router.h>

#include <airmap/logger.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <queue>
#include <set>

namespace airmap {
namespace mavlink {
namespace boost {

// TcpRoute forwards all incoming messages to connected TCP clients.
class TcpRoute : public Router::Route, public std::enable_shared_from_this<TcpRoute> {
 public:
  // Session models an active session with a remote endpoint.
  class Session : public std::enable_shared_from_this<Session> {
   public:
    // create initializes and returns a new instance with 'io_service' and 'logger'.
    static std::shared_ptr<Session> create(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                           const std::shared_ptr<Logger>& logger);

    void process(const mavlink_message_t& message);
    ::boost::asio::ip::tcp::socket& socket();

   private:
    class EncodedBuffer {
     public:
      std::size_t size() const;
      void set_size(std::size_t size);

      unsigned char* data();
      const unsigned char* data() const;

     private:
      std::array<unsigned char, 1024> data_;
      std::size_t size_;
    };

    // Session initializes a new instance with 'io_service' and 'logger'.
    explicit Session(const std::shared_ptr<::boost::asio::io_service>& io_service,
                     const std::shared_ptr<Logger>& logger);

    void process();

    std::shared_ptr<::boost::asio::io_service> io_service_;
    util::FormattingLogger log_;
    ::boost::asio::ip::tcp::socket socket_;
    std::queue<EncodedBuffer> buffers_;
  };

  // Monitor enables users of TcpRoute to inject custom functionality.
  class Monitor : airmap::DoNotCopyOrMove {
   public:
    // on_new_session is invoked for every new 'session'.
    virtual void on_new_session(const std::shared_ptr<Session>& session) = 0;

   protected:
    Monitor() = default;
  };

  // create initializes and returns a new instance with 'io_service', 'endpoint' and 'logger'.
  static std::shared_ptr<TcpRoute> create(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                          const ::boost::asio::ip::tcp::endpoint& endpoint,
                                          const std::shared_ptr<Logger>& logger,
                                          const std::set<std::shared_ptr<Monitor>>& monitors);

  // From Route
  void process(const mavlink_message_t& message) override;
  void start() override;
  void stop() override;

 private:
  // TcpRoute initializes a new instance with 'context'.
  explicit TcpRoute(const std::shared_ptr<::boost::asio::io_service>& io_service,
                    const ::boost::asio::ip::tcp::endpoint& endpoint, const std::shared_ptr<Logger>& logger,
                    const std::set<std::shared_ptr<Monitor>>& monitors);

  void handle_accept(const ::boost::system::error_code& ec);

  std::shared_ptr<::boost::asio::io_service> io_service_;
  ::boost::asio::ip::tcp::acceptor acceptor_;
  util::FormattingLogger log_;
  std::set<std::shared_ptr<Monitor>> monitors_;
  std::set<std::shared_ptr<Session>> sessions_;
};

}  // namespace boost
}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_BOOST_TCP_ROUTE_H_
