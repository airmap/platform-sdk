cmake_minimum_required(VERSION 2.8)

project(external-protobuf)

include(ExternalProject)

ExternalProject_add(
    protobuf
    GIT_REPOSITORY https://github.com/google/protobuf.git
    GIT_TAG v3.5.0
    PREFIX protobuf
    SOURCE_SUBDIR cmake
    CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX=$ENV{AIRMAP_PROTOBUF_OUTPUT_PATH}
        -Dprotobuf_BUILD_TESTS=OFF
        -Dprotobuf_BUILD_SHARED_LIBS=OFF
        -Dprotobuf_WITH_ZLIB=OFF
        -Dprotobuf_MODULE_COMPATIBLE=ON
    UPDATE_COMMAND ""
)

add_custom_target(trigger-protobuf)
add_dependencies(trigger-protobuf protobuf)