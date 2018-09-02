//
//  tcp_route.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/mavlink/boost/tcp_route.h>

namespace {
constexpr const char* component{"TcpRoute"};
}  // namespace

std::shared_ptr<airmap::mavlink::boost::TcpRoute> airmap::mavlink::boost::TcpRoute::create(
    const std::shared_ptr<::boost::asio::io_service>& io_service, const ::boost::asio::ip::tcp::endpoint& endpoint,
    const std::shared_ptr<Logger>& logger, const std::set<std::shared_ptr<Monitor>>& monitors) {
  return std::shared_ptr<TcpRoute>{new TcpRoute{io_service, endpoint, logger, monitors}};
}

airmap::mavlink::boost::TcpRoute::TcpRoute(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                           const ::boost::asio::ip::tcp::endpoint& endpoint,
                                           const std::shared_ptr<Logger>& logger,
                                           const std::set<std::shared_ptr<Monitor>>& monitors)
    : io_service_{io_service}, acceptor_{*io_service_, endpoint}, log_{logger}, monitors_{monitors} {
}

// start starts accepting connections
void airmap::mavlink::boost::TcpRoute::start() {
  auto sp      = shared_from_this();
  auto session = Session::create(io_service_, log_.logger());

  acceptor_.async_accept(session->socket(), [sp, session](const auto& error) {
    if (error) {
      sp->log_.errorf(component, "error accepting incoming connection: %s", error.message());
    } else {
      sp->log_.infof(component, "accepting incoming connection from endpoint: %s:%d",
                     session->socket().remote_endpoint().address().to_string(),
                     session->socket().remote_endpoint().port());
      for (const auto& monitor : sp->monitors_)
        monitor->on_new_session(session);

      sp->sessions_.insert(session);
      sp->start();
    }
  });
}

// stop stops accepting connections
void airmap::mavlink::boost::TcpRoute::stop() {
  acceptor_.cancel();
}

// From Route
void airmap::mavlink::boost::TcpRoute::process(const mavlink_message_t& message) {
  io_service_->post([ sp = shared_from_this(), message ]() {
    for (const auto& session : sp->sessions_)
      session->process(message);
  });
}

std::size_t airmap::mavlink::boost::TcpRoute::Session::EncodedBuffer::size() const {
  return size_;
}

void airmap::mavlink::boost::TcpRoute::Session::EncodedBuffer::set_size(std::size_t size) {
  size_ = size;
}

unsigned char* airmap::mavlink::boost::TcpRoute::Session::EncodedBuffer::data() {
  return data_.data();
}

const unsigned char* airmap::mavlink::boost::TcpRoute::Session::EncodedBuffer::data() const {
  return data_.data();
}

std::shared_ptr<airmap::mavlink::boost::TcpRoute::Session> airmap::mavlink::boost::TcpRoute::Session::create(
    const std::shared_ptr<::boost::asio::io_service>& io_service, const std::shared_ptr<Logger>& logger) {
  return std::shared_ptr<Session>{new Session{io_service, logger}};
}

::boost::asio::ip::tcp::socket& airmap::mavlink::boost::TcpRoute::Session::socket() {
  return socket_;
}

void airmap::mavlink::boost::TcpRoute::Session::process(const mavlink_message_t& message) {
  log_.debugf(component, "processing mavlink message for session on endpoint %s:%d",
              socket_.remote_endpoint().address().to_string(), socket_.remote_endpoint().port());

  EncodedBuffer eb;
  eb.set_size(mavlink_msg_to_send_buffer(eb.data(), &message));

  io_service_->post([ sp = shared_from_this(), eb = std::move(eb) ]() {
    sp->buffers_.emplace(eb);
    if (sp->buffers_.size() == 1)
      sp->process();
  });
}

void airmap::mavlink::boost::TcpRoute::Session::process() {
  const auto& eb = buffers_.front();

  ::boost::asio::async_write(socket_, ::boost::asio::buffer(eb.data(), eb.size()),
                             ::boost::asio::transfer_all(), [sp = shared_from_this()](const auto& error, auto) {
                               sp->buffers_.pop();

                               if (error) {
                                 sp->log_.infof(component,
                                                "failed to process mavlink message for session on endpoint %s:%d: %s",
                                                sp->socket_.remote_endpoint().address().to_string(),
                                                sp->socket_.remote_endpoint().port(), error.message());

                                 return;
                               }

                               if (!sp->buffers_.empty())
                                 sp->process();
                             });
}

airmap::mavlink::boost::TcpRoute::Session::Session(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                                   const std::shared_ptr<Logger>& logger)
    : io_service_{io_service}, log_{logger}, socket_{*io_service_} {
}
