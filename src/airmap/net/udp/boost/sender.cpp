#include <airmap/net/udp/boost/sender.h>

#include <fmt/printf.h>

std::shared_ptr<airmap::net::udp::boost::Sender::Session> airmap::net::udp::boost::Sender::Session::create(
    const std::shared_ptr<::boost::asio::io_service>& io_service, const ::boost::asio::ip::udp::endpoint& endpoint,
    const std::string& message, const Callback& cb) {
  return std::shared_ptr<Session>{new Session{io_service, endpoint, message, cb}};
}

airmap::net::udp::boost::Sender::Session::Session(const std::shared_ptr<::boost::asio::io_service>& io_service,
                                                  const ::boost::asio::ip::udp::endpoint& endpoint,
                                                  const std::string& message, const Callback& cb)
    : io_service_{io_service},
      endpoint_{endpoint},
      socket_{*io_service_, ::boost::asio::ip::udp::endpoint{::boost::asio::ip::udp::v4(), 0}},
      message_{message},
      cb_{cb} {
}

void airmap::net::udp::boost::Sender::Session::start() {
  socket_.async_send_to(
      ::boost::asio::buffer(message_), endpoint_,
      std::bind(&Session::handle_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void airmap::net::udp::boost::Sender::Session::handle_write(const ::boost::system::error_code& ec,
                                                            std::size_t transferred) {
  if (ec) {
    cb_(Result{
        std::make_exception_ptr(std::runtime_error{fmt::sprintf("failed to send udp packet: %s", ec.message())})});
  } else if (transferred != message_.size()) {
    cb_(Result{std::make_exception_ptr(std::runtime_error{"failed to send udp packet"})});
  } else {
    cb_(Result{Empty{}});
  }
}

std::shared_ptr<airmap::net::udp::boost::Sender> airmap::net::udp::boost::Sender::create(
    const std::string& host, std::uint16_t port, const std::shared_ptr<::boost::asio::io_service>& io_service) {
  return std::shared_ptr<Sender>{new Sender{host, port, io_service}};
}

airmap::net::udp::boost::Sender::Sender(const std::string& host, std::uint16_t port,
                                        const std::shared_ptr<::boost::asio::io_service>& io_service)
    : host_{host}, port_{port}, io_service_{io_service}, resolver_{*io_service_} {
}

void airmap::net::udp::boost::Sender::send(const std::string& message, const Callback& cb) {
  auto query = ::boost::asio::ip::udp::resolver::query{host_, std::to_string(port_),
                                                       ::boost::asio::ip::udp::resolver::query::passive};
  resolver_.async_resolve(query, [ this, sp = shared_from_this(), message, cb ](const auto& ec, auto iterator) {
    if (ec) {
      cb(Result{std::make_exception_ptr(std::runtime_error{fmt::sprintf("failed to resolve host: %s", ec.message())})});
    } else {
      Session::create(io_service_, *iterator, message, cb)->start();
    }
  });
}
