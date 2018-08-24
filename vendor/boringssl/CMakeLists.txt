cmake_minimum_required(VERSION 3.5)

project(external-boringssl)

include(ExternalProject)

ExternalProject_add(
    boringssl
    GIT_REPOSITORY https://github.com/google/boringssl.git
    GIT_TAG 38c20fe8d514a5328f5db404e116af8e9136c7f8
    PREFIX boringssl
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=$ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DIOS_PLATFORM=${IOS_PLATFORM}
        -DOPENSSL_NO_ASM=ON # Note: https://github.com/grpc/grpc/issues/9440
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    UPDATE_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include/openssl $ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}/openssl
    COMMAND ${CMAKE_COMMAND} -E copy <BINARY_DIR>/ssl/libssl.a $ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}/lib/libssl.a
    COMMAND ${CMAKE_COMMAND} -E copy <BINARY_DIR>/crypto/libcrypto.a $ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}/lib/libcrypto.a
)

add_custom_target(trigger-boringssl)
add_dependencies(trigger-boringssl boringssl)
