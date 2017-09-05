cmake_minimum_required(VERSION 3.5)
project(airmap)

enable_testing()

find_package(Boost REQUIRED date_time program_options system)
find_package(PkgConfig)
find_package(Protobuf REQUIRED)

pkg_check_modules(OPENSSL REQUIRED openssl)

# All vendored dependencies should be listed here
add_subdirectory(vendor/fmt EXCLUDE_FROM_ALL)
add_subdirectory(vendor/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(vendor/uri EXCLUDE_FROM_ALL)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -pedantic -Wextra -fPIC")

include_directories(
  include
  src

  vendor/beast/include
  vendor/fmt
  vendor/mavlink
  vendor/spdlog/include
  vendor/uri/include
  vendor/mqtt_client_cpp/include

  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src

  ${OPENSSL_INCLUDE_DIRS}
  ${PROTOBUF_INCLUDE_DIRS})

protobuf_generate_cpp(
  TELEMETRY_PROTOBUF_SOURCES
  TELEMETRY_PROTOBUF_HEADERS
  src/airmap/rest/telemetry.proto)

add_library(
  airmap-client

  include/airmap/aircraft.h
  include/airmap/aircrafts.h
  include/airmap/airspace.h
  include/airmap/airspaces.h
  include/airmap/authenticator.h
  include/airmap/client.h
  include/airmap/date_time.h
  include/airmap/flight.h
  include/airmap/flights.h
  include/airmap/optional.h
  include/airmap/pilot.h
  include/airmap/outcome.h
  include/airmap/rule.h
  include/airmap/status.h
  include/airmap/telemetry.h
  include/airmap/timestamp.h
  include/airmap/traffic.h

  src/airmap/airspace.cpp
  src/airmap/client.cpp
  src/airmap/codec.h
  src/airmap/context.cpp
  src/airmap/date_time.cpp
  src/airmap/geometry.cpp
  src/airmap/jsend.h
  src/airmap/jsend.cpp
  src/airmap/logger.cpp
  src/airmap/pilots.cpp
  src/airmap/rule.cpp
  src/airmap/status.cpp
  src/airmap/telemetry.cpp
  src/airmap/traffic.cpp

  src/airmap/daemon.h
  src/airmap/daemon.cpp

  src/airmap/boost/context.h
  src/airmap/boost/context.cpp

  src/airmap/codec/http/query/aircrafts.h
  src/airmap/codec/http/query/aircrafts.cpp
  src/airmap/codec/http/query/airspaces.h
  src/airmap/codec/http/query/airspaces.cpp
  src/airmap/codec/http/query/flights.h
  src/airmap/codec/http/query/flights.cpp
  src/airmap/codec/http/query/pilots.h
  src/airmap/codec/http/query/pilots.cpp
  src/airmap/codec/http/query/status.h
  src/airmap/codec/http/query/status.cpp

  src/airmap/codec/json/aircraft.h
  src/airmap/codec/json/aircraft.cpp
  src/airmap/codec/json/airspace.h
  src/airmap/codec/json/airspace.cpp
  src/airmap/codec/json/authenticator.h
  src/airmap/codec/json/authenticator.cpp
  src/airmap/codec/json/chrono.h
  src/airmap/codec/json/chrono.cpp
  src/airmap/codec/json/date_time.h
  src/airmap/codec/json/date_time.cpp
  src/airmap/codec/json/flight.h
  src/airmap/codec/json/flight.cpp
  src/airmap/codec/json/flights.h
  src/airmap/codec/json/flights.cpp
  src/airmap/codec/json/geometry.h
  src/airmap/codec/json/geometry.cpp
  src/airmap/codec/json/pilot.h
  src/airmap/codec/json/pilot.cpp
  src/airmap/codec/json/pilots.h
  src/airmap/codec/json/pilots.cpp
  src/airmap/codec/json/rule.h
  src/airmap/codec/json/rule.cpp
  src/airmap/codec/json/status.h
  src/airmap/codec/json/status.cpp
  src/airmap/codec/json/traffic.h
  src/airmap/codec/json/traffic.cpp

  src/airmap/net/http/requester.h
  src/airmap/net/http/requester_with_api_key.h
  src/airmap/net/http/requester_with_api_key.cpp
  src/airmap/net/http/boost/request.h
  src/airmap/net/http/boost/request.cpp
  src/airmap/net/http/boost/requester.h
  src/airmap/net/http/boost/requester.cpp
  
  src/airmap/net/mqtt/client.h
  src/airmap/net/mqtt/boost/broker.h
  src/airmap/net/mqtt/boost/broker.cpp
  src/airmap/net/mqtt/boost/client.h
  src/airmap/net/mqtt/boost/client.cpp

  src/airmap/net/udp/sender.h
  src/airmap/net/udp/boost/sender.h
  src/airmap/net/udp/boost/sender.cpp

  src/airmap/rest/client.h
  src/airmap/rest/client.cpp

  src/airmap/rest/aircrafts.h
  src/airmap/rest/aircrafts.cpp
  src/airmap/rest/airspaces.h
  src/airmap/rest/airspaces.cpp
  src/airmap/rest/authenticator.h
  src/airmap/rest/authenticator.cpp
  src/airmap/rest/flights.h
  src/airmap/rest/flights.cpp
  src/airmap/rest/pilots.h
  src/airmap/rest/pilots.cpp
  src/airmap/rest/status.h
  src/airmap/rest/status.cpp
  src/airmap/rest/telemetry.h
  src/airmap/rest/telemetry.cpp
  src/airmap/rest/traffic.h
  src/airmap/rest/traffic.cpp

  src/airmap/mavlink/channel.h
  src/airmap/mavlink/channel.cpp
  src/airmap/mavlink/global_position_int.h
  src/airmap/mavlink/global_position_int.cpp
  src/airmap/mavlink/heartbeat.h
  src/airmap/mavlink/heartbeat.cpp
  src/airmap/mavlink/state.h
  src/airmap/mavlink/state.cpp
  src/airmap/mavlink/vehicle.h
  src/airmap/mavlink/vehicle.cpp
  src/airmap/mavlink/vehicle_tracker.h
  src/airmap/mavlink/vehicle_tracker.cpp
  src/airmap/mavlink/boost/serial_channel.h
  src/airmap/mavlink/boost/serial_channel.cpp
  src/airmap/mavlink/boost/tcp_channel.h
  src/airmap/mavlink/boost/tcp_channel.cpp

  src/airmap/util/cheap_ruler.h
  src/airmap/util/cheap_ruler.cpp
  src/airmap/util/cli.h
  src/airmap/util/cli.cpp
  src/airmap/util/scenario_simulator.h
  src/airmap/util/scenario_simulator.cpp
  src/airmap/util/telemetry_simulator.h
  src/airmap/util/telemetry_simulator.cpp

  ${TELEMETRY_PROTOBUF_SOURCES}
  ${TELEMETRY_PROTOBUF_HEADERS}
)

add_library(
  airmap-server

  src/airmap/daemon.h
  src/airmap/daemon.cpp)

add_executable(
  airmap

  src/airmap/cmds/airmap/airmap.cpp
  src/airmap/cmds/airmap/cmd/authorize_refresh.h
  src/airmap/cmds/airmap/cmd/authorize_refresh.cpp
  src/airmap/cmds/airmap/cmd/authorize_anonymous.h
  src/airmap/cmds/airmap/cmd/authorize_anonymous.cpp
  src/airmap/cmds/airmap/cmd/authorize_password.h
  src/airmap/cmds/airmap/cmd/authorize_password.cpp
  src/airmap/cmds/airmap/cmd/create_flight.h
  src/airmap/cmds/airmap/cmd/create_flight.cpp
  src/airmap/cmds/airmap/cmd/daemon.h
  src/airmap/cmds/airmap/cmd/daemon.cpp
  src/airmap/cmds/airmap/cmd/flags.h
  src/airmap/cmds/airmap/cmd/flags.cpp
  src/airmap/cmds/airmap/cmd/get_status.h
  src/airmap/cmds/airmap/cmd/get_status.cpp
  src/airmap/cmds/airmap/cmd/pilot.h
  src/airmap/cmds/airmap/cmd/pilot.cpp
  src/airmap/cmds/airmap/cmd/simulate_scenario.h
  src/airmap/cmds/airmap/cmd/simulate_scenario.cpp
  src/airmap/cmds/airmap/cmd/simulate_telemetry.h
  src/airmap/cmds/airmap/cmd/simulate_telemetry.cpp
  src/airmap/cmds/airmap/cmd/start_flight_comms.h
  src/airmap/cmds/airmap/cmd/start_flight_comms.cpp
  src/airmap/cmds/airmap/cmd/monitor_traffic.h
  src/airmap/cmds/airmap/cmd/monitor_traffic.cpp)


add_executable(
  airmap-telemetry-simulator

  src/airmap/cmds/airmap_telemetry_simulator/airmap_telemetry_simulator.cpp)

set_property(TARGET airmap-client PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap-server PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap-telemetry-simulator PROPERTY CXX_STANDARD 14)

target_link_libraries(airmap-client fmt network-uri spdlog ${Boost_LIBRARIES} ${OPENSSL_LDFLAGS} ${PROTOBUF_LIBRARIES})
target_link_libraries(airmap airmap-client )
target_link_libraries(airmap-server airmap-client)
target_link_libraries(airmap-telemetry-simulator airmap-client)

find_program(CLANG_FORMAT_EXECUTABLE
  NAMES clang-format clang-format-5.0
        clang-format-4.0 clang-format-3.9
        clang-format-3.8 clang-format-3.7
        clang-format-3.6 clang-format-3.5
        clang-format-3.4 clang-format-3.3
  DOC "clang-format executable"
)

install(TARGETS airmap RUNTIME DESTINATION bin)

if (CLANG_FORMAT_EXECUTABLE)
  file(GLOB_RECURSE AIRMAPD_SRCS include/*.h src/*.h src/*.cpp test/*.h test/*.cpp)

  add_custom_target(
    format ${CLANG_FORMAT_EXECUTABLE} -style=file -i ${AIRMAPD_SRCS})
endif()

add_subdirectory(test)

set(CPACK_PACKAGE_NAME "airmap")
set(CPACK_PACKAGE_FILE_NAME "airmap-installer")
include (CPack)
