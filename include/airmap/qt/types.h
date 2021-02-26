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
#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include <QMetaType>
#include <airmap/types.h>

Q_DECLARE_METATYPE(airmap::Aircraft)
Q_DECLARE_METATYPE(airmap::Airspace)
Q_DECLARE_METATYPE(airmap::Credentials)
Q_DECLARE_METATYPE(airmap::DateTime)
Q_DECLARE_METATYPE(airmap::Error)
Q_DECLARE_METATYPE(airmap::FlightPlan)
Q_DECLARE_METATYPE(airmap::Flight)
Q_DECLARE_METATYPE(airmap::Geometry)
Q_DECLARE_METATYPE(airmap::Pilot)
Q_DECLARE_METATYPE(airmap::Rule)
Q_DECLARE_METATYPE(airmap::RuleSet)
Q_DECLARE_METATYPE(airmap::RuleSet::Rule)
Q_DECLARE_METATYPE(airmap::Status::Advisory)
Q_DECLARE_METATYPE(airmap::Status::Wind)
Q_DECLARE_METATYPE(airmap::Status::Weather)
Q_DECLARE_METATYPE(airmap::Status::Report)
Q_DECLARE_METATYPE(airmap::Telemetry::Position)
Q_DECLARE_METATYPE(airmap::Telemetry::Speed)
Q_DECLARE_METATYPE(airmap::Telemetry::Attitude)
Q_DECLARE_METATYPE(airmap::Telemetry::Barometer)
Q_DECLARE_METATYPE(airmap::Optional<airmap::Telemetry::Update>)
Q_DECLARE_METATYPE(airmap::Token::Type)
Q_DECLARE_METATYPE(airmap::Token::Anonymous)
Q_DECLARE_METATYPE(airmap::Token::OAuth)
Q_DECLARE_METATYPE(airmap::Token::Refreshed)
Q_DECLARE_METATYPE(airmap::Token)
Q_DECLARE_METATYPE(airmap::Traffic::Update::Type)
Q_DECLARE_METATYPE(airmap::Traffic::Update)
Q_DECLARE_METATYPE(airmap::Version)

namespace airmap {
namespace qt {

inline void register_types() {
  qRegisterMetaType<airmap::Aircraft>("Aircraft");
  qRegisterMetaType<airmap::Airspace>("Airspace");
  qRegisterMetaType<airmap::Credentials>("Credentials");
  qRegisterMetaType<airmap::DateTime>("DateTime");
  qRegisterMetaType<airmap::Error>("Error");
  qRegisterMetaType<airmap::FlightPlan>("FlightPlan");
  qRegisterMetaType<airmap::Flight>("Flight");
  qRegisterMetaType<airmap::Geometry>("Geometry");
  qRegisterMetaType<airmap::Pilot>("Pilot");
  qRegisterMetaType<airmap::Rule>("Rule");
  qRegisterMetaType<airmap::RuleSet>("RuleSet");
  qRegisterMetaType<airmap::RuleSet::Rule>("RuleSet::Rule");
  qRegisterMetaType<airmap::Status::Advisory>("Status::Advisory");
  qRegisterMetaType<airmap::Status::Wind>("Status::Wind");
  qRegisterMetaType<airmap::Status::Weather>("Status::Weather");
  qRegisterMetaType<airmap::Status::Report>("Status::Report");
  qRegisterMetaType<airmap::Telemetry::Position>("Telemetry::Position");
  qRegisterMetaType<airmap::Telemetry::Speed>("Telemetry::Speed");
  qRegisterMetaType<airmap::Telemetry::Attitude>("Telemetry::Attitude");
  qRegisterMetaType<airmap::Telemetry::Barometer>("Telemetry::Barometer");
  qRegisterMetaType<airmap::Optional<airmap::Telemetry::Update>>("Optional<Telemetry::Update>");
  qRegisterMetaType<airmap::Token::Type>("Token::Type");
  qRegisterMetaType<airmap::Token::Anonymous>("Token::Anonymous");
  qRegisterMetaType<airmap::Token::OAuth>("Token::OAuth");
  qRegisterMetaType<airmap::Token::Refreshed>("Token::Refreshed");
  qRegisterMetaType<airmap::Token>("Token");
  qRegisterMetaType<airmap::Traffic::Update::Type>("Traffic::Update::Type");
  qRegisterMetaType<airmap::Traffic::Update>("Traffic::Update");
  qRegisterMetaType<airmap::Version>("Version");

  //TODO: what's the difference beween registering airmap::Aircraft named 'Aircraft' and 'airmap::Aircraft'?
  qRegisterMetaType<airmap::Aircraft>("airmap::Aircraft");
  qRegisterMetaType<airmap::Airspace>("airmap::Airspace");
  qRegisterMetaType<airmap::Credentials>("airmap::Credentials");
  qRegisterMetaType<airmap::DateTime>("airmap::DateTime");
  qRegisterMetaType<airmap::Error>("airmap::Error");
  qRegisterMetaType<airmap::FlightPlan>("airmap::FlightPlan");
  qRegisterMetaType<airmap::Flight>("airmap::Flight");
  qRegisterMetaType<airmap::Geometry>("airmap::Geometry");
  qRegisterMetaType<airmap::Pilot>("airmap::Pilot");
  qRegisterMetaType<airmap::Rule>("airmap::Rule");
  qRegisterMetaType<airmap::RuleSet>("airmap::RuleSet");
  qRegisterMetaType<airmap::RuleSet::Rule>("airmap::RuleSet::Rule");
  qRegisterMetaType<airmap::Status::Advisory>("airmap::Advisory");
  qRegisterMetaType<airmap::Status::Wind>("airmap::Wind");
  qRegisterMetaType<airmap::Status::Weather>("airmap::Weather");
  qRegisterMetaType<airmap::Status::Report>("airmap::Report");
  qRegisterMetaType<airmap::Telemetry::Position>("airmap::Telemetry::Position");
  qRegisterMetaType<airmap::Telemetry::Speed>("airmap::Telemetry::Speed");
  qRegisterMetaType<airmap::Telemetry::Attitude>("airmap::Telemetry::Attitude");
  qRegisterMetaType<airmap::Telemetry::Barometer>("airmap::Telemetry::Barometer");
  qRegisterMetaType<airmap::Optional<airmap::Telemetry::Update>>("airmap::Optional<airmap::Telemetry::Update>");
  qRegisterMetaType<airmap::Token::Type>("airmap::Token::Type");
  qRegisterMetaType<airmap::Token::Anonymous>("airmap::Token::Anonymous");
  qRegisterMetaType<airmap::Token::OAuth>("airmap::Token::OAuth");
  qRegisterMetaType<airmap::Token::Refreshed>("airmap::Token::Refreshed");
  qRegisterMetaType<airmap::Token>("airmap::Token");
  qRegisterMetaType<airmap::Traffic::Update::Type>("airmap::Traffic::Update::Type");
  qRegisterMetaType<airmap::Traffic::Update>("airmap::Traffic::Update");
  qRegisterMetaType<airmap::Version>("airmap::Version");
}

}}

#endif // REGISTER_TYPES_H
