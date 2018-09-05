find_package(gRPC CONFIG REQUIRED)

add_library(
  airmap-codec-grpc

  date_time.h
  date_time.cpp
  traffic.h
  traffic.cpp
)

set_property(
  TARGET airmap-codec-grpc
  PROPERTY CXX_STANDARD 14
)

set_property(
  TARGET airmap-codec-grpc
  PROPERTY COMPILE_OPTIONS -fPIC
)

target_include_directories(
  airmap-codec-grpc
  PRIVATE $<TARGET_PROPERTY:gRPC::grpc++,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(
  airmap-codec-grpc

  airmap-interfaces-grpc
)