cmake_minimum_required(VERSION 3.5)
project(airmap)

enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -pedantic -Wextra -fPIC")

find_package(Boost REQUIRED date_time program_options system)
find_package(PkgConfig)
find_package(Protobuf REQUIRED)

pkg_check_modules(LIBSOUP REQUIRED libsoup-2.4)
pkg_check_modules(GIO gio-2.0)
pkg_check_modules(GIO_UNIX gio-unix-2.0)

# cryptopp lacks a pkgconfig setup on OS X and
# we will likely have to adjust the brew formula
set(CRYPTOPP_LIBRARIES cryptopp)

# All vendored dependencies should be listed here
add_subdirectory(vendor/fmt)
add_subdirectory(vendor/spdlog)

include_directories(
  include
  src
  vendor/fmt
  vendor/spdlog/include

  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src

  ${AWS_CPP_SDK_CORE_INCLUDE_DIRS}
  ${GIO_INCLUDE_DIRS}
  ${GIO_UNIX_INCLUDE_DIRS}
  ${LIBSOUP_INCLUDE_DIRS}
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
  include/airmap/telemetry.h
  include/airmap/timestamp.h

  src/airmap/airspace.cpp
  src/airmap/codec.h
  src/airmap/context.cpp
  src/airmap/date_time.cpp
  src/airmap/geometry.cpp
  src/airmap/jsend.h
  src/airmap/jsend.cpp
  src/airmap/logger.cpp
  src/airmap/rule.cpp
  src/airmap/telemetry.cpp

  src/airmap/rest/client.h
  src/airmap/rest/client.cpp
  src/airmap/rest/communicator.h
  src/airmap/rest/glib/communicator.h
  src/airmap/rest/glib/communicator.cpp

  src/airmap/rest/aircrafts.h
  src/airmap/rest/aircrafts.cpp
  src/airmap/rest/airspaces.h
  src/airmap/rest/airspaces.cpp
  src/airmap/rest/authenticator.h
  src/airmap/rest/authenticator.cpp
  src/airmap/rest/flights.h
  src/airmap/rest/flights.cpp
  src/airmap/rest/telemetry.h
  src/airmap/rest/telemetry.cpp

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
  src/airmap/cmds/airmap/cmd/authorize-refresh.h
  src/airmap/cmds/airmap/cmd/authorize-refresh.cpp
  src/airmap/cmds/airmap/cmd/authorize-anonymous.h
  src/airmap/cmds/airmap/cmd/authorize-anonymous.cpp
  src/airmap/cmds/airmap/cmd/authorize-password.h
  src/airmap/cmds/airmap/cmd/authorize-password.cpp
  src/airmap/cmds/airmap/cmd/create_flight.h
  src/airmap/cmds/airmap/cmd/create_flight.cpp
  src/airmap/cmds/airmap/cmd/simulate_scenario.h
  src/airmap/cmds/airmap/cmd/simulate_scenario.cpp
  src/airmap/cmds/airmap/cmd/simulate_telemetry.h
  src/airmap/cmds/airmap/cmd/simulate_telemetry.cpp
  src/airmap/cmds/airmap/cmd/start_flight_comms.h
  src/airmap/cmds/airmap/cmd/start_flight_comms.cpp)

add_executable(
  airmapd

  src/airmap/cmds/airmapd/airmapd.cpp)

add_executable(
  airmap-telemetry-simulator

  src/airmap/cmds/airmap_telemetry_simulator/airmap_telemetry_simulator.cpp)

set_property(TARGET airmap-client PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap-server PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap PROPERTY CXX_STANDARD 14)
set_property(TARGET airmapd PROPERTY CXX_STANDARD 14)
set_property(TARGET airmap-telemetry-simulator PROPERTY CXX_STANDARD 14)

target_link_libraries(airmap-client fmt spdlog ${Boost_LIBRARIES} ${CRYPTOPP_LIBRARIES} ${LIBSOUP_LDFLAGS} ${PROTOBUF_LIBRARIES})
target_link_libraries(airmap airmap-client )
target_link_libraries(airmap-server airmap-client)
target_link_libraries(airmapd airmap-server)
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
