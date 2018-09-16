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