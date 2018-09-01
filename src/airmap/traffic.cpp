//
//  traffic.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/traffic.h>

#include <airmap/util/formatting_logger.h>

#include <iostream>

airmap::Traffic::Monitor::FunctionalSubscriber::FunctionalSubscriber(
    const std::function<void(Traffic::Update::Type, const std::vector<Update>&)>& f)
    : f_{f} {
}

void airmap::Traffic::Monitor::FunctionalSubscriber::handle_update(Traffic::Update::Type type,
                                                                   const std::vector<Update>& updates) {
  f_(type, updates);
}

airmap::Traffic::Monitor::LoggingSubscriber::LoggingSubscriber(const char* component,
                                                               const std::shared_ptr<Logger>& logger)
    : component_{component}, logger_{logger} {
}

void airmap::Traffic::Monitor::LoggingSubscriber::handle_update(Traffic::Update::Type type,
                                                                const std::vector<Update>& updates) {
  util::FormattingLogger log{logger_};
  for (const auto& update : updates) {
    log.infof(component_,
              "traffic update:\n"
              "  type:         %s\n"
              "  id:           %s\n"
              "  aircraft id:  %s\n"
              "  direction:    %s\n"
              "  latitude:     %f\n"
              "  longitude:    %f\n"
              "  altitude:     %f\n"
              "  ground speed: %f\n"
              "  heading:      %f\n"
              "  recorded:     %s\n"
              "  timestamp:    %s",
              type, update.id, update.aircraft_id, update.direction, update.latitude, update.longitude, update.altitude,
              update.ground_speed, update.heading, iso8601::generate(update.recorded),
              iso8601::generate(update.timestamp));
  }
}

std::ostream& airmap::operator<<(std::ostream& out, Traffic::Update::Type type) {
  switch (type) {
    case Traffic::Update::Type::unknown:
      return out << "unknown";
    case Traffic::Update::Type::situational_awareness:
      return out << "situation-awareness";
    case Traffic::Update::Type::alert:
      return out << "alert";
  }

  return out;
}
