#ifndef AIRMAP_NET_MQTT_BOOST_BROKER_H_
#define AIRMAP_NET_MQTT_BOOST_BROKER_H_

#include <airmap/net/mqtt/broker.h>

#include <airmap/logger.h>

#include <boost/asio.hpp>

#include <string>

namespace airmap {
namespace net {
namespace mqtt {
namespace boost {

class Broker : public mqtt::Broker {
 public:
  explicit Broker(const std::string& host, std::uint16_t port, const std::shared_ptr<Logger>& logger,
                  const std::shared_ptr<::boost::asio::io_service>& io_service);
  void connect(const Credentials& credentials, const ConnectCallback& cb);

 private:
  std::shared_ptr<Logger> logger_;
  std::shared_ptr<::boost::asio::io_service> io_service_;
  std::string host_;
  std::uint16_t port_;
};

}  // namespace boost
}  // namespace mqtt
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_MQTT_BOOST_BROKER_H_
