cmake_minimum_required(VERSION 3.12)

project(airmap
  VERSION "1.1.0"
  LANGUAGES C CXX)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

option(AIRMAP_ENABLE_NETWORK_TESTS "enable tests requiring network access"       ON)
option(AIRMAP_ENABLE_GRPC          "Enable libraries/executables requiring gRPC" OFF)
option(AIRMAP_ENABLE_QT            "Enable libraries/executables requiring Qt5"  ON)

if (AIRMAP_ENABLE_GRPC)
  add_definitions(-DAIRMAP_ENABLE_GRPC)
endif ()

find_package(Git)

if (GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE AIRMAPD_VERSION_GIT_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(STATUS "Building in git revision ${AIRMAPD_VERSION_GIT_REVISION}")
else()
  set(AIRMAPD_VERSION_GIT_REVISION "n/a")
  message(STATUS "Executable git is missing, choosing default value for git revision")
endif()


string(TIMESTAMP AIRMAPD_VERSION_BUILD_TIMESTAMP "%Y-%m-%dT%H:%M:%S.000000Z" UTC)
message(STATUS "Building in build timestamp ${AIRMAPD_VERSION_BUILD_TIMESTAMP}")

# Detecting the platform at build time and exposing
# information on the platform to the platform
set(AIRMAP_PLATFORM "null")

if (UNIX)
  # We are treating OS X as a proper Linux-like operating system and store
  # configuration files according to the XDG directory standard.
  set(AIRMAP_PLATFORM "linux")
endif()

message(STATUS "Enabling platform ${AIRMAP_PLATFORM}")

include(external.cmake)
include(airmapd.cmake)
include(pack.cmake)
