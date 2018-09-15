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
//  fan_out_traffic_monitor.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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