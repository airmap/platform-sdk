//
//  vehicle_tracker.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/mavlink/vehicle_tracker.h>

void airmap::mavlink::VehicleTracker::update(const mavlink_message_t& msg) {
  auto it = vehicles_.find(msg.sysid);

  if (it == vehicles_.end()) {
    std::tie(it, std::ignore) = vehicles_.emplace(msg.sysid, std::make_shared<Vehicle>(msg.sysid));
    for (const auto& monitor : monitors_)
      monitor->on_vehicle_added(it->second);
  }

  it->second->update(msg);
}

void airmap::mavlink::VehicleTracker::register_monitor(const std::shared_ptr<Monitor>& monitor) {
  monitors_.insert(monitor);
}

void airmap::mavlink::VehicleTracker::unregister_monitor(const std::shared_ptr<Monitor>& monitor) {
  monitors_.erase(monitor);
}

airmap::mavlink::LoggingVehicleTrackerMonitor::LoggingVehicleTrackerMonitor(
    const char* component, const std::shared_ptr<Logger>& logger, const std::shared_ptr<VehicleTracker::Monitor>& next)
    : component_{component}, log_{logger}, next_{next} {
}

void airmap::mavlink::LoggingVehicleTrackerMonitor::on_vehicle_added(const std::shared_ptr<Vehicle>& vehicle) {
  log_.infof(component_, "vehicle added to tracker: %s", vehicle);
  next_->on_vehicle_added(vehicle);
}

void airmap::mavlink::LoggingVehicleTrackerMonitor::on_vehicle_removed(const std::shared_ptr<Vehicle>& vehicle) {
  log_.infof(component_, "vehicle removed from tracker: %s", vehicle);
  next_->on_vehicle_added(vehicle);
}