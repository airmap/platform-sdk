cmake_minimum_required(VERSION 3.5)

project(airmap C CXX)

set(AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH "${CMAKE_BINARY_DIR}/external")
set(AIRMAP_CARES_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/cares")
set(AIRMAP_GRPC_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/grpc")
set(AIRMAP_PROTOBUF_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/protobuf")

list(APPEND CMAKE_PREFIX_PATH ${AIRMAP_GRPC_OUTPUT_PATH} ${AIRMAP_PROTOBUF_OUTPUT_PATH})

if (EXISTS ${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH})
  include(${AIRMAP_PROTOBUF_OUTPUT_PATH}/lib/cmake/protobuf/protobuf-config.cmake)
  include(${AIRMAP_GRPC_OUTPUT_PATH}/lib/cmake/grpc/gRPCConfig.cmake)
  include(airmapd.cmake)
else()
  message(STATUS "Satisfying external dependencies, please build and re-run cmake after that")
  include(external.cmake)
endif()
