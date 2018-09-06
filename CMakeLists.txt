cmake_minimum_required(VERSION 3.5)
include(prepare_toolchain.cmake)
project(airmap C CXX)

set(AIRMAPD_VERSION_MAJOR 0)
set(AIRMAPD_VERSION_MINOR 0)
set(AIRMAPD_VERSION_PATCH 1)

option(OPENSSL_USE_STATIC_LIBS "Link OpenSSL statically" FALSE)

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
