include(ExternalProject)

ExternalProject_add(
  grpc
  GIT_REPOSITORY https://github.com/grpc/grpc.git
  GIT_TAG v1.7.2
  GIT_PROGRESS 1
  PREFIX grpc
  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${AIRMAP_GRPC_OUTPUT_PATH}
    -DgRPC_BUILD_TESTS=OFF
    -DgRPC_ZLIB_PROVIDER=package
    -DgRPC_CARES_PROVIDER=package
    -DgRPC_SSL_PROVIDER=package
    -DgRPC_PROTOBUF_PROVIDER=package
    -DgRPC_INSTALL_default=ON
  UPDATE_COMMAND ""
)

