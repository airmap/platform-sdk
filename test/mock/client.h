//
//  client.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef MOCK_CLIENT_H_
#define MOCK_CLIENT_H_

#include <airmap/client.h>
#include <airmap/context.h>

#include <airmap/authenticator.h>
#include <airmap/flights.h>
#include <airmap/telemetry.h>

#include <trompeloeil/trompeloeil.hpp>

namespace mock {

using trompeloeil::_;

struct Authenticator : public airmap::Authenticator {
  Authenticator() = default;

  MAKE_MOCK2(authenticate_with_password,
             void(const AuthenticateWithPassword::Params&, const AuthenticateWithPassword::Callback&), override);
  MAKE_MOCK2(authenticate_anonymously,
             void(const AuthenticateAnonymously::Params&, const AuthenticateAnonymously::Callback&), override);
  MAKE_MOCK2(renew_authentication, void(const RenewAuthentication::Params&, const RenewAuthentication::Callback&),
             override);
};

struct Flights : public airmap::Flights {
  Flights() = default;

  MAKE_MOCK2(search, void(const Search::Parameters&, const Search::Callback&), override);
  MAKE_MOCK2(for_id, void(const ForId::Parameters&, const ForId::Callback&), override);
  MAKE_MOCK2(create_flight_by_point, void(const CreateFlight::Parameters&, const CreateFlight::Callback&), override);
  MAKE_MOCK2(create_flight_by_path, void(const CreateFlight::Parameters&, const CreateFlight::Callback&), override);
  MAKE_MOCK2(create_flight_by_polygon, void(const CreateFlight::Parameters&, const CreateFlight::Callback&), override);
  MAKE_MOCK2(end_flight, void(const EndFlight::Parameters&, const EndFlight::Callback&), override);
  MAKE_MOCK2(delete_flight, void(const DeleteFlight::Parameters& parameters, const DeleteFlight::Callback& cb),
             override);
  MAKE_MOCK2(start_flight_communications,
             void(const StartFlightCommunications::Parameters&, const StartFlightCommunications::Callback&), override);
  MAKE_MOCK2(end_flight_communications,
             void(const EndFlightCommunications::Parameters&, const EndFlightCommunications::Callback&), override);
};

struct Telemetry : public airmap::Telemetry {
  Telemetry() = default;

  MAKE_MOCK3(submit_updates, void(const airmap::Flight&, const std::string&, const std::initializer_list<Update>&),
             override);
};

struct Traffic : public airmap::Traffic {
  struct Monitor : public airmap::Traffic::Monitor {
    Monitor() = default;

    MAKE_MOCK1(subscribe, void(const std::shared_ptr<airmap::Traffic::Monitor::Subscriber>&), override);
    MAKE_MOCK1(unsubscribe, void(const std::shared_ptr<airmap::Traffic::Monitor::Subscriber>&), override);
  };

  Traffic() = default;

  MAKE_MOCK2(monitor, void(const airmap::Traffic::Monitor::Params&, const airmap::Traffic::Monitor::Callback&),
             override);
};

struct Client : public airmap::Client {
  MAKE_MOCK0(advisory, airmap::Advisory&(), override);
  MAKE_MOCK0(authenticator, airmap::Authenticator&(), override);
  MAKE_MOCK0(aircrafts, airmap::Aircrafts&(), override);
  MAKE_MOCK0(airspaces, airmap::Airspaces&(), override);
  MAKE_MOCK0(flight_plans, airmap::FlightPlans&(), override);
  MAKE_MOCK0(flights, airmap::Flights&(), override);
  MAKE_MOCK0(pilots, airmap::Pilots&(), override);
  MAKE_MOCK0(rulesets, airmap::RuleSets&(), override);
  MAKE_MOCK0(status, airmap::Status&(), override);
  MAKE_MOCK0(telemetry, airmap::Telemetry&(), override);
  MAKE_MOCK0(traffic, airmap::Traffic&(), override);
};

}  // namespace mock

#endif  // MOCK_CLIENT_H_
