#ifndef AIRMAP_REST_COMMUNICATOR_H_
#define AIRMAP_REST_COMMUNICATOR_H_

#include <airmap/context.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace airmap {
namespace mqtt {

/// QualityOfService models all known
/// QoS values supported by mqtt.
enum class QualityOfService { at_least_once, at_most_once, exactly_once };

/// Client models access to an MQTT broker, and enables
/// calling code to subscribe to topics.
class Client : DoNotCopyOrMove {
 public:
  /// PublishCallback is invoked for incoming publishs.
  /// @param topic the topic of the publish
  /// @param contents the contents of the publish
  using PublishCallback = std::function<void(const std::string&, const std::string&)>;

  /// An instance of Subscription models an individual subscription to a
  /// topic. Deleting the instance removes the specific subscription.
  class Subscription : DoNotCopyOrMove {
   protected:
    Subscription() = default;
  };

  /// subscribe subscribes the caller to 'topic', with 'qos', invoking cb
  /// for incoming publishs of 'topic'.
  virtual std::unique_ptr<Subscription> subscribe(const std::string& topic, QualityOfService qos,
                                                  PublishCallback cb) = 0;

 protected:
  Client() = default;
};

}  // namespace mqtt

namespace rest {

class Communicator : DoNotCopyOrMove {
 public:
  using Status = std::uint16_t;

  using ConnectResult   = Outcome<std::shared_ptr<mqtt::Client>, std::exception_ptr>;
  using ConnectCallback = std::function<void(const ConnectResult&)>;
  using CreateResult    = Outcome<std::shared_ptr<Communicator>, std::exception_ptr>;
  using DoResult        = Outcome<std::string, std::exception_ptr>;
  using DoCallback      = std::function<void(const DoResult&)>;

  virtual void connect_to_mqtt_broker(const std::string& host, std::uint16_t port, const std::string& username,
                                      const std::string& password, const ConnectCallback& cb) = 0;
  virtual void delete_(const std::string& host, const std::string& path,
                       std::unordered_map<std::string, std::string>&& query,
                       std::unordered_map<std::string, std::string>&& headers, DoCallback cb) = 0;
  virtual void get(const std::string& host, const std::string& path,
                   std::unordered_map<std::string, std::string>&& query,
                   std::unordered_map<std::string, std::string>&& headers, DoCallback cb)     = 0;

  virtual void post(const std::string& host, const std::string& path,
                    std::unordered_map<std::string, std::string>&& headers, const std::string& body, DoCallback cb) = 0;
  virtual void patch(const std::string& host, const std::string& path,
                     std::unordered_map<std::string, std::string>&& headers, const std::string& body,
                     DoCallback cb)                                                                                 = 0;
  virtual void send_udp(const std::string& host, std::uint16_t port, const std::string& body)                       = 0;

  virtual void dispatch(const std::function<void()>& task) = 0;

 protected:
  Communicator() = default;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_COMMUNICATOR_H_
