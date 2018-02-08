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

set(
  AIRMAP_TELEMETRY_PROTOBUF_PROTOS

  ${CMAKE_SOURCE_DIR}/vendor/airmap/am-protos/protos/telemetry/amd_uav_flight_update.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/rest/telemetry.proto
)

set(
  AIRMAP_TELEMETRY_PROTOBUF_SOURCES

  ${CMAKE_CURRENT_BINARY_DIR}/telemetry.pb.h
  ${CMAKE_CURRENT_BINARY_DIR}/telemetry.pb.cc
  ${CMAKE_CURRENT_BINARY_DIR}/amd_uav_flight_update.pb.h
  ${CMAKE_CURRENT_BINARY_DIR}/amd_uav_flight_update.pb.cc
)

add_custom_command(
  OUTPUT ${AIRMAP_TELEMETRY_PROTOBUF_SOURCES}

  COMMAND $<TARGET_FILE:protobuf::protoc>
    --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
    --proto_path ${CMAKE_SOURCE_DIR}/vendor/airmap/am-protos/protos/telemetry/
    --proto_path ${CMAKE_CURRENT_SOURCE_DIR}/rest/
    ${AIRMAP_TELEMETRY_PROTOBUF_PROTOS}

  DEPENDS $<TARGET_FILE:protobuf::protoc> ${AIRMAP_TELEMETRY_PROTOBUF_PROTOS}
  COMMENT "Generating C++ code for AirMap telemetry interfaces"
  VERBATIM)

set_source_files_properties(
  ${AIRMAP_TELEMETRY_PROTOBUF_SOURCES}
  PROPERTIES GENERATED TRUE
)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/interfaces
)

add_library(
  airmap-mavlink

  mavlink/channel.h
  mavlink/channel.cpp
  mavlink/global_position_int.h
  mavlink/global_position_int.cpp
  mavlink/heartbeat.h
  mavlink/heartbeat.cpp
  mavlink/router.h
  mavlink/router.cpp
  mavlink/state.h
  mavlink/state.cpp
  mavlink/vehicle.h
  mavlink/vehicle.cpp
  mavlink/vehicle_tracker.h
  mavlink/vehicle_tracker.cpp
  mavlink/boost/serial_channel.h
  mavlink/boost/serial_channel.cpp
  mavlink/boost/tcp_channel.h
  mavlink/boost/tcp_channel.cpp
  mavlink/boost/tcp_route.h
  mavlink/boost/tcp_route.cpp
  mavlink/boost/udp_channel.h
  mavlink/boost/udp_channel.cpp
)

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

  codec/grpc/date_time.h
  codec/grpc/date_time.cpp
  codec/grpc/traffic.h
  codec/grpc/traffic.cpp

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
  net/http/requester_with_api_key.h
  net/http/requester_with_api_key.cpp
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

  ${AIRMAP_TELEMETRY_PROTOBUF_SOURCES}
  ${AIRMAP_TELEMETRY_AMD_UAV_FLIGHT_UPDATE_PROTOBUF_SOURCES}
  ${AIRMAP_TELEMETRY_AMD_UAV_FLIGHT_UPDATE_PROTOBUF_HEADERS}
)

add_library(
  airmap-grpc

  grpc/init.h
  grpc/init.cpp
  grpc/method_invocation.h
  grpc/method_invocation.cpp

  grpc/client/executor.h
  grpc/client/executor.cpp

  grpc/server/executor.h
  grpc/server/executor.cpp
  grpc/server/service.h
)

add_library(
  airmap-monitor

  monitor/daemon.h
  monitor/daemon.cpp
  monitor/fan_out_traffic_monitor.h
  monitor/fan_out_traffic_monitor.cpp
  monitor/submitting_vehicle_monitor.h
  monitor/submitting_vehicle_monitor.cpp
  monitor/telemetry_submitter.h
  monitor/telemetry_submitter.cpp

  monitor/grpc/client.h
  monitor/grpc/client.cpp
  monitor/grpc/service.h
  monitor/grpc/service.cpp
)

add_library(
  airmap-cpp SHARED

  $<TARGET_OBJECTS:airmap-client>
)

add_library(
  airmap-qt SHARED

  $<TARGET_OBJECTS:airmap-client>

  qt/client.cpp
  qt/dispatcher.h
  qt/dispatcher.cpp
  qt/logger.cpp

  qt/advisory.h
  qt/advisory.cpp
  qt/aircrafts.h
  qt/aircrafts.cpp
  qt/airspaces.h
  qt/airspaces.cpp
  qt/authenticator.h
  qt/authenticator.cpp
  qt/flight_plans.h
  qt/flight_plans.cpp
  qt/flights.h
  qt/flights.cpp
  qt/pilots.h
  qt/pilots.cpp
  qt/rulesets.h
  qt/rulesets.cpp
  qt/status.h
  qt/status.cpp
  qt/telemetry.h
  qt/telemetry.cpp
  qt/traffic.h
  qt/traffic.cpp
  qt/types.cpp
)

add_executable(
  airmap

  $<TARGET_OBJECTS:airmap-client>

  cmds/airmap/airmap.cpp
  cmds/airmap/cmd/add_aircraft.h
  cmds/airmap/cmd/add_aircraft.cpp
  cmds/airmap/cmd/aircraft_models.h
  cmds/airmap/cmd/aircraft_models.cpp
  cmds/airmap/cmd/create_flight.h
  cmds/airmap/cmd/create_flight.cpp
  cmds/airmap/cmd/daemon.h
  cmds/airmap/cmd/daemon.cpp
  cmds/airmap/cmd/end_flight.h
  cmds/airmap/cmd/end_flight.cpp
  cmds/airmap/cmd/end_flight_comms.h
  cmds/airmap/cmd/end_flight_comms.cpp
  cmds/airmap/cmd/evaluate_rulesets.h
  cmds/airmap/cmd/evaluate_rulesets.cpp
  cmds/airmap/cmd/fetch_rules.h
  cmds/airmap/cmd/fetch_rules.cpp
  cmds/airmap/cmd/flags.h
  cmds/airmap/cmd/flags.cpp
  cmds/airmap/cmd/get_advisories.h
  cmds/airmap/cmd/get_advisories.cpp
  cmds/airmap/cmd/get_status.h
  cmds/airmap/cmd/get_status.cpp
  cmds/airmap/cmd/init.h
  cmds/airmap/cmd/init.cpp
  cmds/airmap/cmd/login.h
  cmds/airmap/cmd/login.cpp
  cmds/airmap/cmd/pilot.h
  cmds/airmap/cmd/pilot.cpp
  cmds/airmap/cmd/plan_flight.h
  cmds/airmap/cmd/plan_flight.cpp
  cmds/airmap/cmd/query_rulesets.h
  cmds/airmap/cmd/query_rulesets.cpp
  cmds/airmap/cmd/render_briefing.h
  cmds/airmap/cmd/render_briefing.cpp
  cmds/airmap/cmd/report_weather.h
  cmds/airmap/cmd/report_weather.cpp
  cmds/airmap/cmd/simulate_scenario.h
  cmds/airmap/cmd/simulate_scenario.cpp
  cmds/airmap/cmd/simulate_telemetry.h
  cmds/airmap/cmd/simulate_telemetry.cpp
  cmds/airmap/cmd/start_flight_comms.h
  cmds/airmap/cmd/start_flight_comms.cpp
  cmds/airmap/cmd/submit_flight.h
  cmds/airmap/cmd/submit_flight.cpp
  cmds/airmap/cmd/monitor_mids.h
  cmds/airmap/cmd/monitor_mids.cpp
  cmds/airmap/cmd/monitor_traffic.h
  cmds/airmap/cmd/monitor_traffic.cpp
  cmds/airmap/cmd/test.h
  cmds/airmap/cmd/test.cpp
  cmds/airmap/cmd/test/laanc.phoenix.h
  cmds/airmap/cmd/test/laanc.phoenix.cpp
  cmds/airmap/cmd/version.h
  cmds/airmap/cmd/version.cpp
)

target_include_directories(
  airmap-client
  PRIVATE $<TARGET_PROPERTY:gRPC::grpc++,INTERFACE_INCLUDE_DIRECTORIES>)

set_property(
  TARGET airmap-client
  PROPERTY CXX_STANDARD 14)
set_property(
  TARGET airmap-client
  PROPERTY COMPILE_OPTIONS -fPIC)
set_property(
  TARGET airmap-grpc
  PROPERTY CXX_STANDARD 14)
set_property(
  TARGET airmap-grpc
  PROPERTY COMPILE_OPTIONS -fPIC)
set_property(
  TARGET airmap-monitor
  PROPERTY CXX_STANDARD 14)
set_property(
  TARGET airmap-monitor
  PROPERTY COMPILE_OPTIONS -fPIC)

set_target_properties(
  airmap-cpp PROPERTIES

  CXX_STANDARD      14
  SOVERSION         "${AIRMAPD_VERSION_MAJOR}.${AIRMAPD_VERSION_MINOR}.${AIRMAPD_VERSION_PATCH}"
  VERSION           "${AIRMAPD_VERSION_MAJOR}"
)

set_target_properties(
  airmap-qt PROPERTIES

  CXX_STANDARD      14
  SOVERSION         "${AIRMAPD_VERSION_MAJOR}.${AIRMAPD_VERSION_MINOR}.${AIRMAPD_VERSION_PATCH}"
  VERSION           "${AIRMAPD_VERSION_MAJOR}"
)

set_property(
  TARGET airmap-mavlink
  PROPERTY CXX_STANDARD 14)
set_property(
  TARGET airmap-mavlink
  PROPERTY COMPILE_OPTIONS "-Wno-pedantic")
set_property(
  TARGET airmap
  PROPERTY CXX_STANDARD 14)

target_link_libraries(
  airmap-grpc

  airmap-interfaces-grpc
)

target_link_libraries(
  airmap-monitor

  airmap-grpc
  airmap-mavlink
)

target_link_libraries(
  airmap-cpp

  airmap-grpc
  airmap-monitor

  ${Boost_LIBRARIES}

  OpenSSL::Crypto
  OpenSSL::SSL
)

target_link_libraries(
  airmap-qt

  airmap-grpc
  airmap-monitor

  ${Boost_LIBRARIES}

  OpenSSL::Crypto
  OpenSSL::SSL

  Qt5::Core
)

target_link_libraries(
  airmap

  airmap-grpc
  airmap-monitor

  ${Boost_LIBRARIES}

  OpenSSL::Crypto
  OpenSSL::SSL
)

install(
  TARGETS airmap
  RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  TARGETS airmap-cpp
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  TARGETS airmap-qt
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/airmap
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)