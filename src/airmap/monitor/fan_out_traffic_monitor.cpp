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
