cmake_minimum_required(VERSION 3.5)

project(external-protobuf C CXX)

include(ExternalProject)

ExternalProject_add(
    protobuf
    GIT_REPOSITORY https://github.com/airmap/protobuf.git
    GIT_SHALLOW 1
    PREFIX protobuf
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
      -DCMAKE_INSTALL_PREFIX:PATH=$ENV{AIRMAP_PROTOBUF_OUTPUT_PATH}
      -Dprotobuf_BUILD_TESTS=OFF
      -Dprotobuf_BUILD_SHARED_LIBS=OFF
      -Dprotobuf_WITH_ZLIB=OFF
      -Dprotobuf_MODULE_COMPATIBLE=ON
      <SOURCE_DIR>/cmake
    UPDATE_COMMAND ""
)

add_custom_target(trigger-protobuf)
add_dependencies(trigger-protobuf protobuf)