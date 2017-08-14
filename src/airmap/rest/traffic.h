#ifndef AIRMAP_REST_TRAFFIC_H_
#define AIRMAP_REST_TRAFFIC_H_

#include <airmap/traffic.h>

#include <airmap/logger.h>
#include <airmap/rest/communicator.h>
#include <airmap/util/formatting_logger.h>

#include <set>
#include <vector>

namespace airmap {
namespace rest {

class Traffic : public airmap::Traffic {
 public:
  class Monitor : public airmap::Traffic::Monitor {
   public:
    explicit Monitor(const std::shared_ptr<Logger>& logger, const std::string& flight_id, const std::shared_ptr<mqtt::Client>& client);

    void subscribe(const std::shared_ptr<Subscriber>& subscriber) override;
    void unsubscribe(const std::shared_ptr<Subscriber>& subscriber) override;

   private:
    bool handle_suback(std::uint16_t packet_id, std::vector<::boost::optional<std::uint8_t>> subs);
    bool handle_publish(std::uint8_t header, ::boost::optional<std::uint16_t> packet_id, std::string topic_name,
                        std::string contents);

    util::FormattingLogger log_;
    std::string flight_id_;
    std::shared_ptr<mqtt::Client> mqtt_client_;
    std::set<std::shared_ptr<Subscriber>> subscribers_;
    std::uint8_t sa_subscription_id_;
    std::uint8_t alert_subscription_id_;
  };

  explicit Traffic(const std::shared_ptr<Logger>& logger, const std::string& host, std::uint16_t port, Communicator& communicator);

  void monitor(const Monitor::Params& params, const Monitor::Callback& cb) override;

 private:
  util::FormattingLogger log_;
  std::string host_;
  std::uint16_t port_;
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_TRAFFIC_H_
