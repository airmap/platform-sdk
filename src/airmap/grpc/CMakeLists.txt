add_library(
  airmap-grpc STATIC

  init.h
  init.cpp
  method_invocation.h
  method_invocation.cpp

  client/executor.h
  client/executor.cpp

  server/executor.h
  server/executor.cpp
  server/service.h
)

set_property(
  TARGET airmap-grpc
  PROPERTY CXX_STANDARD 14
)

set_property(
  TARGET airmap-grpc
  PROPERTY COMPILE_OPTIONS -fPIC
)

target_include_directories(
  airmap-grpc
  PRIVATE $<TARGET_PROPERTY:gRPC::grpc++,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(
  airmap-grpc

  airmap-interfaces-grpc
)