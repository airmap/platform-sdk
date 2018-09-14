cmake_minimum_required(VERSION 3.5)

project(external-protobuf C CXX)

include(ExternalProject)

if (${CMAKE_CROSSCOMPILING})
  set(AIRMAP_PROTOBUF_BUILD_PROTOC_BINARIES OFF)
else ()
  set(AIRMAP_PROTOBUF_BUILD_PROTOC_BINARIES ON)
endif ()

ExternalProject_add(
    protobuf
    URL https://github.com/google/protobuf/releases/download/v3.6.1/protobuf-cpp-3.6.1.tar.gz
    PREFIX protobuf
    PATCH_COMMAND
      patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/patches/fix-linking-for-android.patch
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
      -DCMAKE_INSTALL_PREFIX:PATH=$ENV{AIRMAP_PROTOBUF_OUTPUT_PATH}
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE
      -DANDROID_ABI=${ANDROID_ABI}
      -DANDROID_STL=${ANDROID_STL}
      -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
      -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
      -Dprotobuf_BUILD_PROTOC_BINARIES=${AIRMAP_PROTOBUF_BUILD_PROTOC_BINARIES}
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