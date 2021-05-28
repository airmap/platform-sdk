set(
  AIRMAP_PLATFORM_SRCS
  platform/null/interface.h
  platform/null/interface.cpp
  platform/null/standard_paths.h
  platform/null/standard_paths.cpp
)

if (${AIRMAP_PLATFORM} STREQUAL "linux")
  add_definitions(-DAIRMAP_PLATFORM_LINUX)
  set(
    AIRMAP_PLATFORM_SRCS
    ${AIRMAP_PLATFORM_SRCS}
    platform/linux/interface.h
    platform/linux/interface.cpp
    platform/linux/standard_paths.h
    platform/linux/standard_paths.cpp
    platform/linux/xdg.h
    platform/linux/xdg.cpp
  )
  set(AIRMAP_PLATFORM_LIBRARIES ${AIRMAP_PLATFORM_LIBRARIES} xdg)
else()
  add_definitions(-DAIRMAP_PLATFORM_NULL)
endif()

configure_file(version.cpp.in version.cpp)

add_library(
  airmap-client OBJECT

  ${CMAKE_SOURCE_DIR}/include/airmap/advisory.h
  ${CMAKE_SOURCE_DIR}/include/airmap/aircraft.h
  ${CMAKE_SOURCE_DIR}/include/airmap/aircrafts.h
  ${CMAKE_SOURCE_DIR}/include/airmap/airspace.h
  ${CMAKE_SOURCE_DIR}/include/airmap/airspaces.h
  ${CMAKE_SOURCE_DIR}/include/airmap/authenticator.h
  ${CMAKE_SOURCE_DIR}/include/airmap/client.h
  ${CMAKE_SOURCE_DIR}/include/airmap/credentials.h
  ${CMAKE_SOURCE_DIR}/include/airmap/date_time.h
  ${CMAKE_SOURCE_DIR}/include/airmap/evaluation.h
  ${CMAKE_SOURCE_DIR}/include/airmap/flight.h
  ${CMAKE_SOURCE_DIR}/include/airmap/flights.h
  ${CMAKE_SOURCE_DIR}/include/airmap/optional.h
  ${CMAKE_SOURCE_DIR}/include/airmap/pilot.h
  ${CMAKE_SOURCE_DIR}/include/airmap/outcome.h
  ${CMAKE_SOURCE_DIR}/include/airmap/rule.h
  ${CMAKE_SOURCE_DIR}/include/airmap/ruleset.h
  ${CMAKE_SOURCE_DIR}/include/airmap/rulesets.h
  ${CMAKE_SOURCE_DIR}/include/airmap/status.h
  ${CMAKE_SOURCE_DIR}/include/airmap/telemetry.h
  ${CMAKE_SOURCE_DIR}/include/airmap/timestamp.h
  ${CMAKE_SOURCE_DIR}/include/airmap/traffic.h

  airspace.cpp
  client.cpp
  codec.h
  context.cpp
  credentials.cpp
  date_time.cpp
  error.cpp
  evaluation.cpp
  geometry.cpp
  jsend.h
  logger.cpp
  paths.cpp
  pilots.cpp
  rule.cpp
  ruleset.cpp
  status.cpp
  telemetry.cpp
  token.cpp
  traffic.cpp

  boost/context.h
  boost/context.cpp

  codec/http/query/advisories.h
  codec/http/query/advisories.cpp
  codec/http/query/aircrafts.h
  codec/http/query/aircrafts.cpp
  codec/http/query/airspaces.h
  codec/http/query/airspaces.cpp
  codec/http/query/flights.h
  codec/http/query/flights.cpp
  codec/http/query/pilots.h
  codec/http/query/pilots.cpp
  codec/http/query/rulesets.h
  codec/http/query/rulesets.cpp
  codec/http/query/status.h
  codec/http/query/status.cpp

  codec/json/advisory.h
  codec/json/advisory.cpp
  codec/json/advisories.h
  codec/json/advisories.cpp
  codec/json/aircraft.h
  codec/json/aircraft.cpp
  codec/json/airspace.h
  codec/json/airspace.cpp
  codec/json/authenticator.h
  codec/json/authenticator.cpp
  codec/json/chrono.h
  codec/json/chrono.cpp
  codec/json/client.h
  codec/json/client.cpp
  codec/json/credentials.h
  codec/json/credentials.cpp
  codec/json/date_time.h
  codec/json/date_time.cpp
  codec/json/evaluation.h
  codec/json/evaluation.cpp
  codec/json/flight_plan.h
  codec/json/flight_plan.cpp
  codec/json/flight_plans.h
  codec/json/flight_plans.cpp
  codec/json/flight.h
  codec/json/flight.cpp
  codec/json/flights.h
  codec/json/flights.cpp
  codec/json/geometry.h
  codec/json/geometry.cpp
  codec/json/pilot.h
  codec/json/pilot.cpp
  codec/json/pilots.h
  codec/json/pilots.cpp
  codec/json/rule.h
  codec/json/rule.cpp
  codec/json/ruleset.h
  codec/json/ruleset.cpp
  codec/json/rulesets.h
  codec/json/rulesets.cpp
  codec/json/status.h
  codec/json/status.cpp
  codec/json/token.h
  codec/json/token.cpp
  codec/json/traffic.h
  codec/json/traffic.cpp

  net/http/requester.h
  net/http/requester.cpp
  net/http/authorized_requester.h
  net/http/authorized_requester.cpp
  net/http/response.h
  net/http/response.cpp
  net/http/user_agent.h
  net/http/user_agent.cpp
  net/http/boost/request.h
  net/http/boost/request.cpp
  net/http/boost/requester.h
  net/http/boost/requester.cpp

  net/mqtt/client.h
  net/mqtt/boost/broker.h
  net/mqtt/boost/broker.cpp
  net/mqtt/boost/client.h
  net/mqtt/boost/client.cpp

  net/udp/sender.h
  net/udp/boost/sender.h
  net/udp/boost/sender.cpp

  rest/client.h
  rest/client.cpp

  rest/advisory.h
  rest/advisory.cpp
  rest/aircrafts.h
  rest/aircrafts.cpp
  rest/airspaces.h
  rest/airspaces.cpp
  rest/authenticator.h
  rest/authenticator.cpp
  rest/flight_plans.h
  rest/flight_plans.cpp
  rest/flights.h
  rest/flights.cpp
  rest/pilots.h
  rest/pilots.cpp
  rest/rulesets.h
  rest/rulesets.cpp
  rest/status.h
  rest/status.cpp
  rest/telemetry.h
  rest/telemetry.cpp
  rest/traffic.h
  rest/traffic.cpp

  platform/interface.h
  platform/interface.cpp
  platform/standard_paths.cpp
  platform/standard_paths.h

  util/cheap_ruler.h
  util/cheap_ruler.cpp
  util/cli.h
  util/cli.cpp
  util/scenario_simulator.h
  util/scenario_simulator.cpp
  util/telemetry_simulator.h
  util/telemetry_simulator.cpp

  ${CMAKE_CURRENT_BINARY_DIR}/version.cpp

  ${AIRMAP_PLATFORM_SRCS}

  pregenerated/amd_uav_flight_update.pb.h
  pregenerated/amd_uav_flight_update.pb.cc
  pregenerated/telemetry.pb.h
  pregenerated/telemetry.pb.cc
)

set_property(
  TARGET airmap-client
  PROPERTY CXX_STANDARD 14
)

set_property(
  TARGET airmap-client
  PROPERTY COMPILE_OPTIONS -fPIC
)

target_compile_options(
  airmap-client

  PRIVATE -Wno-c++11-narrowing
)

add_subdirectory(cmds/airmap)
add_subdirectory(cpp)
add_subdirectory(mavlink)

if (AIRMAP_ENABLE_GRPC)
  add_subdirectory(codec/grpc)
  add_subdirectory(grpc)
  add_subdirectory(monitor)

  target_include_directories(
    airmap-client
    
    PUBLIC ${CMAKE_BINARY_DIR}/interfaces
    PUBLIC ${CMAKE_BINARY_DIR}/interfaces/grpc
  )
endif ()

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/airmap
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)