set(AIRMAP_INTERFACES_GRPC_MESSAGE_PROTOS
    airmap/geometry.proto
    airmap/ids.proto
    airmap/traffic.proto
    airmap/units.proto
    airmap/wgs84.proto)

set(AIRMAP_INTERFACES_GRPC_SERVICE_PROTOS
    airmap/monitor/monitor.proto) 

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/interfaces)

set(AIRMAP_INTERFACES_GRPC_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/geometry.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/geometry.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/ids.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/ids.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/traffic.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/traffic.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/units.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/units.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/wgs84.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/wgs84.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/monitor/monitor.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/monitor/monitor.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/monitor/monitor.grpc.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/airmap/monitor/monitor.grpc.pb.cc)

add_custom_command(
  OUTPUT ${AIRMAP_INTERFACES_GRPC_SRCS}

  COMMAND $<TARGET_FILE:protobuf::protoc>
    --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
    --proto_path ${CMAKE_CURRENT_SOURCE_DIR}
    ${AIRMAP_INTERFACES_GRPC_MESSAGE_PROTOS} ${AIRMAP_INTERFACES_GRPC_SERVICE_PROTOS}

  COMMAND $<TARGET_FILE:protobuf::protoc> 
    --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
    --proto_path ${CMAKE_CURRENT_SOURCE_DIR}
    --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    ${AIRMAP_INTERFACES_GRPC_SERVICE_PROTOS}

  DEPENDS $<TARGET_FILE:protobuf::protoc> $<TARGET_FILE:gRPC::grpc_cpp_plugin> ${AIRMAP_INTERFACES_GRPC_PROTOS}
  COMMENT "Generating C++ code for AirMap gRPC interfaces"
  VERBATIM)

set_source_files_properties(
  ${AIRMAP_INTERFACES_GRPC_SRCS}
  PROPERTIES GENERATED TRUE
)

add_library(
    airmap-interfaces-grpc STATIC
    ${AIRMAP_INTERFACES_GRPC_SRCS}
)

set_property(
    TARGET airmap-interfaces-grpc 
    PROPERTY CXX_STANDARD 14
)

target_compile_options(
    airmap-interfaces-grpc 
    PUBLIC -Wno-extended-offsetof -Wno-unused-parameter -fPIC
)

target_include_directories(
    airmap-interfaces-grpc 
    PUBLIC ${CMAKE_BINARY_DIR}/interfaces
    PUBLIC ${CMAKE_BINARY_DIR}/interfaces/grpc
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    airmap-interfaces-grpc
    gRPC::grpc++
)