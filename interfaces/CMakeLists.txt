set(AIRMAP_INTERFACES_GRPC_MESSAGE_PROTOS
    grpc/airmap/geometry.proto
    grpc/airmap/ids.proto
    grpc/airmap/traffic.proto
    grpc/airmap/units.proto
    grpc/airmap/wgs84.proto)

set(AIRMAP_INTERFACES_GRPC_SERVICE_PROTOS
    grpc/airmap/monitor/monitor.proto) 

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/interfaces)

set(AIRMAP_INTERFACES_GRPC_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/geometry.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/geometry.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/ids.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/ids.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/traffic.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/traffic.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/units.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/units.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/wgs84.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/wgs84.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/monitor/monitor.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/monitor/monitor.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/monitor/monitor.grpc.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/grpc/airmap/monitor/monitor.grpc.pb.cc)

add_custom_command(
  OUTPUT ${AIRMAP_INTERFACES_GRPC_SRCS}

  COMMAND ${Protobuf_PROTOC_EXECUTABLE} 
    --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
    --proto_path ${CMAKE_CURRENT_SOURCE_DIR}
    ${AIRMAP_INTERFACES_GRPC_MESSAGE_PROTOS} ${AIRMAP_INTERFACES_GRPC_SERVICE_PROTOS}

  COMMAND ${Protobuf_PROTOC_EXECUTABLE} 
    --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
    --proto_path ${CMAKE_CURRENT_SOURCE_DIR}
    --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} 
    ${AIRMAP_INTERFACES_GRPC_SERVICE_PROTOS}

  DEPENDS ${Protobuf_PROTOC_EXECUTABLE} ${GRPC_CPP_PLUGIN} ${AIRMAP_INTERFACES_GRPC_PROTOS}
  COMMENT "Generating C++ code for AirMap gRPC interfaces"
  VERBATIM)

set_source_files_properties(
  ${AIRMAP_INTERFACES_GRPC_SRCS}
  PROPERTIES GENERATED TRUE
)

add_library(
    airmap-interfaces-grpc
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
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    airmap-interfaces-grpc
    PUBLIC gRPC::grpc++
)