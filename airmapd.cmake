include(CTest)

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(AIRMAP_ENABLE_NETWORK_TESTS "enable tests requiring network access" ON)

find_package(Boost 1.65.1 REQUIRED date_time filesystem program_options system)
find_package(PkgConfig)
find_package(Qt5Core)

pkg_check_modules(OPENSSL REQUIRED openssl)

# vendor-specific setup goes here
add_definitions(-DFMT_HEADER_ONLY)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -pedantic -Wextra -Wno-implicit-fallthrough -fPIC")

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

  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIRS})

add_subdirectory(doc)
add_subdirectory(interfaces)
add_subdirectory(examples)
add_subdirectory(src/airmap)
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
  file(
    GLOB_RECURSE AIRMAPD_SRCS
    examples/*.h examples/*.cpp
    include/*.h 
    interfaces/*.proto
    src/*.h src/*.cpp
    test/*.h test/*.cpp)

  add_custom_target(
    format ${CLANG_FORMAT_EXECUTABLE} -style=file -i ${AIRMAPD_SRCS})
else()
  message(STATUS "Enabling format target")
endif()

set(CPACK_PACKAGE_NAME "airmap")
set(CPACK_PACKAGE_FILE_NAME "airmap-installer")
include (CPack)
