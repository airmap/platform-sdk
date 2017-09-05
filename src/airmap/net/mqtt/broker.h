#ifndef AIRMAP_NET_MQTT_BROKER_H_
#define AIRMAP_NET_MQTT_BROKER_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <functional>
#include <memory>
#include <string>

namespace airmap {
namespace net {
namespace mqtt {

class Client;

class Broker : public DoNotCopyOrMove {
 public:
  using ConnectResult   = Outcome<std::shared_ptr<Client>, std::exception_ptr>;
  using ConnectCallback = std::function<void(const ConnectResult&)>;

  struct Credentials {
    std::string username;
    std::string password;
  };

  virtual void connect(const Credentials& credentials, const ConnectCallback& cb) = 0;

 protected:
  Broker() = default;
};

}  // namespace mqtt
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_MQTT_BROKER_H_
