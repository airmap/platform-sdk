#include <airmap/monitor/fan_out_traffic_monitor.h>

void airmap::monitor::FanOutTrafficMonitor::subscribe(const std::shared_ptr<Traffic::Monitor::Subscriber>& subscriber) {
  std::lock_guard<std::mutex> lg{subscribers_guard_};
  subscribers_.insert(subscriber);
}

void airmap::monitor::FanOutTrafficMonitor::unsubscribe(
    const std::shared_ptr<Traffic::Monitor::Subscriber>& subscriber) {
  std::lock_guard<std::mutex> lg{subscribers_guard_};
  subscribers_.erase(subscriber);
}

void airmap::monitor::FanOutTrafficMonitor::handle_update(Traffic::Update::Type type,
                                                          const std::vector<Traffic::Update>& update) {
  std::set<std::shared_ptr<Traffic::Monitor::Subscriber>> copy;
  {
    std::lock_guard<std::mutex> lg{subscribers_guard_};
    copy = subscribers_;
  }
  for (const auto& subscriber : copy)
    subscriber->handle_update(type, update);
}