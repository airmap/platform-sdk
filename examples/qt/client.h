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
#ifndef AIRMAP_EXAMPLES_QT_CLIENT_H_
#define AIRMAP_EXAMPLES_QT_CLIENT_H_

#include <airmap/qt/types.h>

#include <QObject>

namespace airmap {
namespace examples {
namespace qt {

class Client : public QObject {
  Q_OBJECT
 public:
  Client(QObject* parent);

  bool test();

  Q_SIGNAL void sig_(const Aircraft&);
  Q_SIGNAL void sig_(const Airspace&);
  Q_SIGNAL void sig_(const Credentials&);
  Q_SIGNAL void sig_(const DateTime&);
  Q_SIGNAL void sig_(const FlightPlan&);
  Q_SIGNAL void sig_(const Flight&);
  Q_SIGNAL void sig_(const Geometry&);
  Q_SIGNAL void sig_(const Pilot&);
  Q_SIGNAL void sig_(const Rule&);
  Q_SIGNAL void sig_(const RuleSet&);
  Q_SIGNAL void sig_(const RuleSet::Rule&);
  Q_SIGNAL void sig_(const Status::Advisory&);
  Q_SIGNAL void sig_(const Status::Wind&);
  Q_SIGNAL void sig_(const Status::Weather&);
  Q_SIGNAL void sig_(const Status::Report&);
  Q_SIGNAL void sig_(const Telemetry::Position&);
  Q_SIGNAL void sig_(const Telemetry::Speed&);
  Q_SIGNAL void sig_(const Telemetry::Attitude&);
  Q_SIGNAL void sig_(const Telemetry::Barometer&);
  Q_SIGNAL void sig_(const Optional<Telemetry::Update>&);
  Q_SIGNAL void sig_(const Token::Type&);
  Q_SIGNAL void sig_(const Token::Anonymous&);
  Q_SIGNAL void sig_(const Token::OAuth&);
  Q_SIGNAL void sig_(const Token::Refreshed&);
  Q_SIGNAL void sig_(const Token&);
  Q_SIGNAL void sig_(const Traffic::Update::Type&);
  Q_SIGNAL void sig_(const Traffic::Update&);
  Q_SIGNAL void sig_(const Version&);

  Q_SLOT void slot_(const Aircraft&);
  Q_SLOT void slot_(const Airspace&);
  Q_SLOT void slot_(const Credentials&);
  Q_SLOT void slot_(const DateTime&);
  Q_SLOT void slot_(const FlightPlan&);
  Q_SLOT void slot_(const Flight&);
  Q_SLOT void slot_(const Geometry&);
  Q_SLOT void slot_(const Pilot&);
  Q_SLOT void slot_(const Rule&);
  Q_SLOT void slot_(const RuleSet&);
  Q_SLOT void slot_(const RuleSet::Rule&);
  Q_SLOT void slot_(const Status::Advisory&);
  Q_SLOT void slot_(const Status::Wind&);
  Q_SLOT void slot_(const Status::Weather&);
  Q_SLOT void slot_(const Status::Report&);
  Q_SLOT void slot_(const Telemetry::Position&);
  Q_SLOT void slot_(const Telemetry::Speed&);
  Q_SLOT void slot_(const Telemetry::Attitude&);
  Q_SLOT void slot_(const Telemetry::Barometer&);
  Q_SLOT void slot_(const Optional<Telemetry::Update>&);
  Q_SLOT void slot_(const Token::Type&);
  Q_SLOT void slot_(const Token::Anonymous&);
  Q_SLOT void slot_(const Token::OAuth&);
  Q_SLOT void slot_(const Token::Refreshed&);
  Q_SLOT void slot_(const Token&);
  Q_SLOT void slot_(const Traffic::Update::Type&);
  Q_SLOT void slot_(const Traffic::Update&);
  Q_SLOT void slot_(const Version&);
};

}  // namespace qt
}  // namespace examples
}  // namespace airmap

#endif  // AIRMAP_EXAMPLES_QT_CLIENT_H_
