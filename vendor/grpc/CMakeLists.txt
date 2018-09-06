cmake_minimum_required(VERSION 3.5)

project(external-grpc C CXX)

include(ExternalProject)

ExternalProject_add(
    grpc
    GIT_REPOSITORY https://github.com/dronecore/grpc.git
    GIT_TAG enable-android-ios-build
    GIT_SUBMODULES third_party/benchmark third_party/gflags
    PREFIX grpc
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=$ENV{AIRMAP_GRPC_OUTPUT_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DIOS_PLATFORM=${IOS_PLATFORM}
        -DRUN_HAVE_STD_REGEX=0
        -DRUN_HAVE_POSIX_REGEX=0
        -DRUN_HAVE_STEADY_CLOCK=0
        -DgRPC_BUILD_CODEGEN=OFF
        -DgRPC_BUILD_TESTS=OFF
        -DgRPC_ZLIB_PROVIDER=package
        -DgRPC_CARES_PROVIDER=package
        -DgRPC_SSL_PROVIDER=package
        -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
        -DOPENSSL_USE_STATIC_LIBS=${OPENSSL_USE_STATIC_LIBS}
        -DgRPC_PROTOBUF_PROVIDER=package
        -DgRPC_PROTOBUF_PACKAGE_TYPE=CONFIG
        -DgRPC_INSTALL_default=ON
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    UPDATE_COMMAND ""
)

add_custom_target(trigger-grpc)
add_dependencies(trigger-grpc grpc)
