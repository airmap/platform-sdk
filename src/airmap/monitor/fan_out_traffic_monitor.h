#ifndef AIRMAP_MONITOR_FAN_OUT_TRAFFIC_MONITOR_H_
#define AIRMAP_MONITOR_FAN_OUT_TRAFFIC_MONITOR_H_

#include <airmap/traffic.h>

#include <memory>
#include <mutex>
#include <set>

namespace airmap {
namespace monitor {

// FanOutTrafficMonitor fans out incoming updates to all of its subscribers.
class FanOutTrafficMonitor : public Traffic::Monitor::Subscriber, public Traffic::Monitor {
 public:
  // From Traffic::Monitor
  void subscribe(const std::shared_ptr<Traffic::Monitor::Subscriber>& subscriber) override;
  void unsubscribe(const std::shared_ptr<Traffic::Monitor::Subscriber>& subscriber) override;

  // From Traffic::Monitor::Subscriber
  void handle_update(airmap::Traffic::Update::Type type, const std::vector<airmap::Traffic::Update>& update) override;

 protected:
  std::mutex subscribers_guard_;
  std::set<std::shared_ptr<Traffic::Monitor::Subscriber>> subscribers_;
};

}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_MONITOR_FAN_OUT_TRAFFIC_MONITOR_H_