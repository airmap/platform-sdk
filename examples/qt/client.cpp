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
//  client.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include "client.h"

#include <airmap/qt/client.h>
#include <airmap/qt/logger.h>
#include <airmap/qt/types.h>

#include <airmap/authenticator.h>

#include <QCoreApplication>

namespace {

constexpr const char* api_key =
    "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
    "eyJjcmVkZW50aWFsX2lkIjoiY3JlZGVudGlhbHx6MzN6T0dEY21wbjV3NmZHTVdwUFpJblBEd1puIiwiYXBwbGljYXRpb25faWQiOiJhcHBsaWNh"
    "dGlvbnxwNE4ySnF2aGc3cFpxWVM2dzZ2eFh0Z2czeDV4Iiwib3JnYW5pemF0aW9uX2lkIjoiZGV2ZWxvcGVyfEt5cURrM0p0N2VuUGJLVVk0QU5i"
    "NUllMEFXbkQiLCJpYXQiOjE1MDg4ODU1NTN9.K3ejcgnoyip3u59ba-VBCivs6tn5gahOsI9FYkCI464";
}  // namespace

int main(int argc, char** argv) {
  QCoreApplication app{argc, argv};

  airmap::qt::register_types();

  auto client  = new airmap::examples::qt::Client{&app};
  auto qlogger = std::make_shared<airmap::qt::Logger>();

  if (!client->test()) {
    qCritical("Failed to establish queued signal-slot connections, exiting with error");
    exit(1);
  }

  qlogger->logging_category().setEnabled(QtDebugMsg, true);
  qlogger->logging_category().setEnabled(QtInfoMsg, true);
  qlogger->logging_category().setEnabled(QtWarningMsg, true);

  auto credentials    = airmap::Credentials{};
  credentials.api_key = api_key;
  auto dlogger        = std::make_shared<airmap::qt::DispatchingLogger>(qlogger);
  auto configuration  = airmap::Client::default_production_configuration(credentials);

  airmap::qt::Client::create(configuration, dlogger, &app, [](const auto& result) {
    if (result) {
      qInfo("Successfully created airmap::qt::Client instance");
      airmap::Authenticator::AuthenticateAnonymously::Params params;
      params.id = "qt client";
      result.value()->authenticator().authenticate_anonymously(params, [](const auto& result) {
        if (result) {
          qInfo("Successfully authenticated with AirMap: %s", result.value().id.c_str());
          QCoreApplication::exit(0);
        } else {
          qInfo("Failed to authenticate with AirMap");
          QCoreApplication::exit(1);
        }
      });
    } else {
      qInfo("Failed to create airmap::qt::Client instance");
      QCoreApplication::exit(1);
    }
  });

  return app.exec();
}

airmap::examples::qt::Client::Client(QObject* parent) : QObject{parent} {
}

bool airmap::examples::qt::Client::test() {
  bool result = true;
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Aircraft&)), this, SLOT(slot_(const Aircraft&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Airspace&)), this, SLOT(slot_(const Airspace&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Credentials&)), this, SLOT(slot_(const Credentials&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const DateTime&)), this, SLOT(slot_(const DateTime&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const FlightPlan&)), this, SLOT(slot_(const FlightPlan&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Flight&)), this, SLOT(slot_(const Flight&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Geometry&)), this, SLOT(slot_(const Geometry&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Pilot&)), this, SLOT(slot_(const Pilot&)), Qt::QueuedConnection));
  result &=
      static_cast<bool>(connect(this, SIGNAL(sig_(const Rule&)), this, SLOT(slot_(const Rule&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const RuleSet&)), this, SLOT(slot_(const RuleSet&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const RuleSet::Rule&)), this, SLOT(slot_(const RuleSet::Rule&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Status::Advisory&)), this,
                                      SLOT(slot_(const Status::Advisory&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Status::Wind&)), this, SLOT(slot_(const Status::Wind&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Status::Weather&)), this,
                                      SLOT(slot_(const Status::Weather&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Status::Report&)), this,
                                      SLOT(slot_(const Status::Report&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Telemetry::Position&)), this,
                                      SLOT(slot_(const Telemetry::Position&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Telemetry::Speed&)), this,
                                      SLOT(slot_(const Telemetry::Speed&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Telemetry::Attitude&)), this,
                                      SLOT(slot_(const Telemetry::Attitude&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Telemetry::Barometer&)), this,
                                      SLOT(slot_(const Telemetry::Barometer&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Optional<Telemetry::Update>&)), this,
                                      SLOT(slot_(const Optional<Telemetry::Update>&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Token::Type&)), this, SLOT(slot_(const Token::Type&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Token::Anonymous&)), this,
                                      SLOT(slot_(const Token::Anonymous&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Token::OAuth&)), this, SLOT(slot_(const Token::OAuth&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Token::Refreshed&)), this,
                                      SLOT(slot_(const Token::Refreshed&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Token&)), this, SLOT(slot_(const Token&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Traffic::Update::Type&)), this,
                                      SLOT(slot_(const Traffic::Update::Type&)), Qt::QueuedConnection));
  result &= static_cast<bool>(connect(this, SIGNAL(sig_(const Traffic::Update&)), this,
                                      SLOT(slot_(const Traffic::Update&)), Qt::QueuedConnection));
  result &= static_cast<bool>(
      connect(this, SIGNAL(sig_(const Version&)), this, SLOT(slot_(const Version&)), Qt::QueuedConnection));

  return result;
}

void airmap::examples::qt::Client::slot_(const Aircraft&) {
}
void airmap::examples::qt::Client::slot_(const Airspace&) {
}
void airmap::examples::qt::Client::slot_(const Credentials&) {
}
void airmap::examples::qt::Client::slot_(const DateTime&) {
}
void airmap::examples::qt::Client::slot_(const FlightPlan&) {
}
void airmap::examples::qt::Client::slot_(const Flight&) {
}
void airmap::examples::qt::Client::slot_(const Geometry&) {
}
void airmap::examples::qt::Client::slot_(const Pilot&) {
}
void airmap::examples::qt::Client::slot_(const Rule&) {
}
void airmap::examples::qt::Client::slot_(const RuleSet&) {
}
void airmap::examples::qt::Client::slot_(const RuleSet::Rule&) {
}
void airmap::examples::qt::Client::slot_(const Status::Advisory&) {
}
void airmap::examples::qt::Client::slot_(const Status::Wind&) {
}
void airmap::examples::qt::Client::slot_(const Status::Weather&) {
}
void airmap::examples::qt::Client::slot_(const Status::Report&) {
}
void airmap::examples::qt::Client::slot_(const Telemetry::Position&) {
}
void airmap::examples::qt::Client::slot_(const Telemetry::Speed&) {
}
void airmap::examples::qt::Client::slot_(const Telemetry::Attitude&) {
}
void airmap::examples::qt::Client::slot_(const Telemetry::Barometer&) {
}
void airmap::examples::qt::Client::slot_(const Optional<Telemetry::Update>&) {
}
void airmap::examples::qt::Client::slot_(const Token::Type&) {
}
void airmap::examples::qt::Client::slot_(const Token::Anonymous&) {
}
void airmap::examples::qt::Client::slot_(const Token::OAuth&) {
}
void airmap::examples::qt::Client::slot_(const Token::Refreshed&) {
}
void airmap::examples::qt::Client::slot_(const Token&) {
}
void airmap::examples::qt::Client::slot_(const Traffic::Update::Type&) {
}
void airmap::examples::qt::Client::slot_(const Traffic::Update&) {
}
void airmap::examples::qt::Client::slot_(const Version&) {
}
