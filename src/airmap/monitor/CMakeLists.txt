add_library(
  airmap-monitor STATIC

  daemon.h
  daemon.cpp
  fan_out_traffic_monitor.h
  fan_out_traffic_monitor.cpp
  submitting_vehicle_monitor.h
  submitting_vehicle_monitor.cpp
  telemetry_submitter.h
  telemetry_submitter.cpp

  grpc/client.h
  grpc/client.cpp
  grpc/service.h
  grpc/service.cpp
)

set_property(
  TARGET airmap-monitor
  PROPERTY CXX_STANDARD 14
)

set_property(
  TARGET airmap-monitor
  PROPERTY COMPILE_OPTIONS -fPIC
)

target_include_directories(
  airmap-monitor
  PRIVATE $<TARGET_PROPERTY:gRPC::grpc++,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(
  airmap-monitor

  airmap-grpc
  airmap-codec-grpc
  airmap-mavlink
)