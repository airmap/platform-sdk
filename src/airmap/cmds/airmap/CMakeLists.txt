if (AIRMAP_ENABLE_GRPC)
  list(
    APPEND CONDITIONAL_SOURCES

    cmd/daemon.h
    cmd/daemon.cpp
  )
endif ()

add_executable(
  airmap

  $<TARGET_OBJECTS:airmap-client>

  airmap.cpp
  cmd/add_aircraft.h
  cmd/add_aircraft.cpp
  cmd/aircraft_models.h
  cmd/aircraft_models.cpp
  cmd/create_flight.h
  cmd/create_flight.cpp
  cmd/end_flight.h
  cmd/end_flight.cpp
  cmd/end_flight_comms.h
  cmd/end_flight_comms.cpp
  cmd/evaluate_rulesets.h
  cmd/evaluate_rulesets.cpp
  cmd/fetch_rules.h
  cmd/fetch_rules.cpp
  cmd/flags.h
  cmd/flags.cpp
  cmd/get_advisories.h
  cmd/get_advisories.cpp
  cmd/get_status.h
  cmd/get_status.cpp
  cmd/init.h
  cmd/init.cpp
  cmd/login.h
  cmd/login.cpp
  cmd/pilot.h
  cmd/pilot.cpp
  cmd/plan_flight.h
  cmd/plan_flight.cpp
  cmd/query_rulesets.h
  cmd/query_rulesets.cpp
  cmd/render_briefing.h
  cmd/render_briefing.cpp
  cmd/report_weather.h
  cmd/report_weather.cpp
  cmd/search_airspace.h
  cmd/search_airspace.cpp
  cmd/simulate_scenario.h
  cmd/simulate_scenario.cpp
  cmd/simulate_telemetry.h
  cmd/simulate_telemetry.cpp
  cmd/start_flight_comms.h
  cmd/start_flight_comms.cpp
  cmd/submit_flight.h
  cmd/submit_flight.cpp
  cmd/monitor_mids.h
  cmd/monitor_mids.cpp
  cmd/monitor_traffic.h
  cmd/monitor_traffic.cpp
  cmd/test.h
  cmd/test.cpp
  cmd/test/laanc.phoenix.h
  cmd/test/laanc.phoenix.cpp
  cmd/version.h
  cmd/version.cpp

  ${CONDITIONAL_SOURCES}
)

set_property(
  TARGET airmap
  PROPERTY CXX_STANDARD 14
)

if (AIRMAP_ENABLE_GRPC)
  list(
    APPEND CONDITIONAL_LIBRARIES
    airmap-grpc
    airmap-monitor
  )

  target_include_directories(
    airmap
    
    PUBLIC ${CMAKE_BINARY_DIR}/interfaces
    PUBLIC ${CMAKE_BINARY_DIR}/interfaces/grpc
  )
endif()

target_link_libraries(
  airmap
  airmap-mavlink

  ${CONDITIONAL_LIBRARIES}

  ${Boost_LIBRARIES}

  OpenSSL::Crypto
  OpenSSL::SSL

  protobuf::libprotobuf
)

install(
  TARGETS airmap
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)