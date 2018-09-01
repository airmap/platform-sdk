//
//  telemetry.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/telemetry.h>

airmap::Telemetry::Update::Update(const Position& position) : type_{Type::position} {
  data_.position = position;
}

airmap::Telemetry::Update::Update(const Speed& speed) : type_{Type::speed} {
  data_.speed = speed;
}

airmap::Telemetry::Update::Update(const Attitude& attitude) : type_{Type::attitude} {
  data_.attitude = attitude;
}

airmap::Telemetry::Update::Update(const Barometer& barometer) : type_{Type::barometer} {
  data_.barometer = barometer;
}

airmap::Telemetry::Update::Type airmap::Telemetry::Update::type() const {
  return type_;
}

const airmap::Telemetry::Position& airmap::Telemetry::Update::position() const {
  return data_.position;
}

const airmap::Telemetry::Speed& airmap::Telemetry::Update::speed() const {
  return data_.speed;
}

const airmap::Telemetry::Attitude& airmap::Telemetry::Update::attitude() const {
  return data_.attitude;
}

const airmap::Telemetry::Barometer& airmap::Telemetry::Update::barometer() const {
  return data_.barometer;
}