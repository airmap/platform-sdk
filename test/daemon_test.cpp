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
#define BOOST_TEST_MODULE daemon

#include <airmap/boost/context.h>
#include <airmap/date_time.h>
#include <airmap/mavlink/boost/tcp_channel.h>
#include <airmap/mavlink/boost/tcp_route.h>
#include <airmap/mavlink/router.h>
#include <airmap/monitor/daemon.h>

#include <mock/client.h>

#include <boost/test/included/unit_test.hpp>

#include <thread>

namespace ip = boost::asio::ip;
using tcp    = boost::asio::ip::tcp;

BOOST_AUTO_TEST_CASE(daemon_creates_flights_for_state_change_to_active) {
  using mock::_;

  static constexpr const std::uint8_t system_id    = 42;
  static constexpr const std::uint8_t component_id = 84;
  static constexpr const std::uint32_t custom_mode = 0;
  static const std::string api_key                 = "api_key";
  static const std::string user_id                 = "id";
  static const std::string aircraft_id             = "aircraft_id";
  static const std::string flight_id               = "flight_id";
  static const std::string key                     = "key";

  auto position_message = []() -> mavlink_message_t {
    mavlink_message_t msg;
    mavlink_msg_global_position_int_pack(system_id, component_id, &msg,
                                         airmap::microseconds_since_epoch(airmap::Clock::universal_time()), 35 * 1E7,
                                         -78 * 1E7, 106 * 1E3, 220 * 1E3, 3 * 1E2, 4 * 1E2, 5 * 1E2, 180 * 1E2);
    return msg;
  };

  std::vector<std::function<mavlink_message_t()>> recording = {
      position_message,
      []() -> mavlink_message_t {
        mavlink_message_t msg;
        mavlink_msg_heartbeat_pack(system_id, component_id, &msg, MAV_TYPE_HELICOPTER, MAV_AUTOPILOT_GENERIC,
                                   MAV_MODE_GUIDED_ARMED, custom_mode, MAV_STATE_ACTIVE);
        return msg;
      },
      position_message};

  constexpr const std::uint16_t port = 9090;
  auto logger                        = airmap::create_default_logger(std::cout);
  auto context                       = airmap::boost::Context::create(logger);
  auto tcp_route =
      airmap::mavlink::boost::TcpRoute::create(context->io_service(), tcp::endpoint{tcp::v4(), port}, logger, {});

  airmap::mavlink::Router router{{tcp_route}};
  router.start();

  auto search_flight_response = airmap::Flights::Search::Response{};
  auto search_flights_result  = airmap::Flights::Search::Result{search_flight_response};

  auto token                 = airmap::Token::Anonymous{user_id};
  auto authentication_result = airmap::Authenticator::AuthenticateAnonymously::Result{token};

  auto flight               = airmap::Flight{};
  flight.id                 = flight_id;
  auto create_flight_result = airmap::Flights::CreateFlight::Result{flight};

  auto response                  = airmap::Flights::StartFlightCommunications::Response{};
  response.key                   = key;
  auto start_flight_comms_result = airmap::Flights::StartFlightCommunications::Result{response};

  mock::Authenticator authenticator;
  REQUIRE_CALL(authenticator, authenticate_anonymously(_, _)).SIDE_EFFECT(_2(authentication_result));

  mock::Flights flights;
  REQUIRE_CALL(flights, search(_, _)).SIDE_EFFECT(_2(search_flights_result));
  REQUIRE_CALL(flights, create_flight_by_point(_, _)).SIDE_EFFECT(_2(create_flight_result));
  REQUIRE_CALL(flights, start_flight_communications(_, _)).SIDE_EFFECT(_2(start_flight_comms_result));

  mock::Telemetry telemetry;
  REQUIRE_CALL(telemetry, submit_updates(_, _, _)).LR_SIDE_EFFECT(context->stop(airmap::Context::ReturnCode::success));

  mock::Traffic traffic;
  auto traffic_monitor = std::make_shared<mock::Traffic::Monitor>();
  REQUIRE_CALL(*traffic_monitor, subscribe(_));
  auto monitor_result = airmap::Traffic::Monitor::Result{traffic_monitor};
  REQUIRE_CALL(traffic, monitor(_, _)).SIDE_EFFECT(_2(monitor_result));

  auto client = std::make_shared<mock::Client>();

  REQUIRE_CALL(*client, authenticator()).LR_RETURN(std::ref(authenticator));
  REQUIRE_CALL(*client, flights()).LR_RETURN(std::ref(flights)).TIMES(AT_LEAST(1));
  REQUIRE_CALL(*client, telemetry()).LR_RETURN(std::ref(telemetry));
  REQUIRE_CALL(*client, traffic()).LR_RETURN(std::ref(traffic));

  auto channel = std::make_shared<airmap::mavlink::boost::TcpChannel>(logger, context->io_service(),
                                                                      ip::address::from_string("127.0.0.1"), port);

  airmap::Credentials::Anonymous anon;
  anon.id = user_id;

  airmap::Credentials credentials;
  credentials.api_key   = api_key;
  credentials.anonymous = anon;

  airmap::monitor::Daemon::Configuration config{credentials, aircraft_id, logger,        channel,
                                                context,     client,      "0.0.0.0:9292"};

  auto daemon = airmap::monitor::Daemon::create(config);
  daemon->start();

  std::thread worker{[context]() { context->run(); }};

  std::this_thread::sleep_for(std::chrono::milliseconds{500});

  for (const auto& step : recording) {
    router.route(step());
  }

  if (worker.joinable())
    worker.join();
}
