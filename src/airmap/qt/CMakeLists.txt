add_library(
  airmap-qt SHARED

  $<TARGET_OBJECTS:airmap-client>

  client.cpp
  dispatcher.h
  dispatcher.cpp
  logger.cpp

  advisory.h
  advisory.cpp
  aircrafts.h
  aircrafts.cpp
  airspaces.h
  airspaces.cpp
  authenticator.h
  authenticator.cpp
  flight_plans.h
  flight_plans.cpp
  flights.h
  flights.cpp
  pilots.h
  pilots.cpp
  rulesets.h
  rulesets.cpp
  status.h
  status.cpp
  telemetry.h
  telemetry.cpp
  traffic.h
  traffic.cpp
  types.cpp
)

set_target_properties(
  airmap-qt PROPERTIES

  CXX_STANDARD      14
  SOVERSION         "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
  VERSION           "${PROJECT_VERSION_MAJOR}"
)

if (AIRMAP_ENABLE_GRPC)
  list(
    APPEND CONDITIONAL_LIBRARIES
    airmap-codec-grpc
    airmap-grpc
    airmap-monitor
  )
endif ()

target_link_libraries(
  airmap-qt
  airmap-mavlink

  ${CONDITIONAL_LIBRARIES}

  ${Boost_LIBRARIES}

  OpenSSL::Crypto
  OpenSSL::SSL
  ${WE_NEED_BORINGSSLS_LIB_DECREPIT}

  protobuf::libprotobuf

  Qt5::Core
)

install(
  TARGETS airmap-qt
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)