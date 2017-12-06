cmake_minimum_required(VERSION 3.5)
project(airmap)

set(AIRMAPD_VERSION_MAJOR 0)
set(AIRMAPD_VERSION_MINOR 0)
set(AIRMAPD_VERSION_PATCH 1)

option(AIRMAP_ENABLE_NETWORK_TESTS "enable tests requiring network access" ON)

find_program(GIT_EXECUTABLE
  NAMES git
  DOC "git executable"
)

if (GIT_EXECUTABLE)
  execute_process(
    COMMAND git rev-parse HEAD
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

enable_testing()

include(GNUInstallDirs)

set(Boost_USE_STATIC_LIBS ON)
set(Protobuf_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED date_time filesystem program_options system unit_test_framework)
find_package(PkgConfig)
find_package(Protobuf REQUIRED)
find_package(Qt5Core)

pkg_check_modules(OPENSSL REQUIRED openssl)

# Detecting the platform at build time and exposing
# information on the platform to the platform
set(AIRMAP_PLATFORM "null")

if (UNIX)
  # We are treating OS X as a proper Linux-like operating system and store
  # configuration files according to the XDG directory standard.
  set(AIRMAP_PLATFORM "linux")
endif()

message(STATUS "Enabling platform ${AIRMAP_PLATFORM}")

# All vendored dependencies should be listed here

add_definitions(-DFMT_HEADER_ONLY)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -pedantic -Wextra -fPIC")

include_directories(
  include
  src
  test

  vendor/
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

add_subdirectory(doc)
add_subdirectory(src/airmap)
add_subdirectory(examples)
add_subdirectory(test)

find_program(CLANG_FORMAT_EXECUTABLE
  NAMES clang-format clang-format-5.0
        clang-format-4.0 clang-format-3.9
        clang-format-3.8 clang-format-3.7
        clang-format-3.6 clang-format-3.5
        clang-format-3.4 clang-format-3.3
  DOC "clang-format executable"
)

if (CLANG_FORMAT_EXECUTABLE)
  message(STATUS "Enabling format target")
  file(GLOB_RECURSE AIRMAPD_SRCS include/*.h src/*.h src/*.cpp test/*.h test/*.cpp)

  add_custom_target(
    format ${CLANG_FORMAT_EXECUTABLE} -style=file -i ${AIRMAPD_SRCS})
else()
  message(STATUS "Enabling format target")
endif()

set(CPACK_PACKAGE_NAME "airmap")
set(CPACK_PACKAGE_FILE_NAME "airmap-installer")
include (CPack)
