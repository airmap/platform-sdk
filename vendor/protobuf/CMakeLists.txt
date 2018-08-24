cmake_minimum_required(VERSION 3.5)

project(external-protobuf C CXX)

include(ExternalProject)

ExternalProject_add(
    protobuf
    GIT_REPOSITORY https://github.com/google/protobuf.git
    GIT_TAG ab95b1bc1eb964ee5f1fb48297344c5d37d35191
    PREFIX protobuf
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
      -DCMAKE_INSTALL_PREFIX:PATH=$ENV{AIRMAP_PROTOBUF_OUTPUT_PATH}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
      -Dprotobuf_BUILD_PROTOC_BINARIES=OFF
      -Dprotobuf_BUILD_TESTS=OFF
      -Dprotobuf_BUILD_SHARED_LIBS=OFF
      -Dprotobuf_WITH_ZLIB=OFF
      -Dprotobuf_MODULE_COMPATIBLE=ON
      <SOURCE_DIR>/cmake
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    UPDATE_COMMAND ""
)

add_custom_target(trigger-protobuf)
add_dependencies(trigger-protobuf protobuf)
