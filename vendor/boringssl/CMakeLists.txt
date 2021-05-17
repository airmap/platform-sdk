cmake_minimum_required(VERSION 3.5)

project(external-boringssl)

include(CheckCXXCompilerFlag)
include(ExternalProject)

check_cxx_compiler_flag(
  "-Wno-tautological-constant-compare"
  COMPILER_SUPPORTS_NO_TAUTOLOGICAL_CONSTANT_COMPARE
)

if (COMPILER_SUPPORTS_NO_TAUTOLOGICAL_CONSTANT_COMPARE)
  set(AIRMAP_BORINGSSL_CXX_FLAGS "-Wno-tautological-constant-compare")
endif ()

ExternalProject_add(
    boringssl
    GIT_REPOSITORY https://github.com/google/boringssl.git
    GIT_TAG ddecaabdc8c950d1417ed69785ac17c3400bae4c
    PREFIX boringssl
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_PLATFORM=${ANDROID_PLATFORM}
        -DANDROID_STL=${ANDROID_STL}
        -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
        -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DCMAKE_CXX_FLAGS=${AIRMAP_BORINGSSL_CXX_FLAGS}
        -DOPENSSL_NO_ASM=ON # Note: https://github.com/grpc/grpc/issues/9440
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    UPDATE_COMMAND ""
    INSTALL_COMMAND 
      ${CMAKE_COMMAND}                 -E copy_directory  <SOURCE_DIR>/include/openssl        $ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}/include/openssl
      COMMAND         ${CMAKE_COMMAND} -E copy            <BINARY_DIR>/ssl/libssl.a           $ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}/lib/libssl.a
      COMMAND         ${CMAKE_COMMAND} -E copy            <BINARY_DIR>/crypto/libcrypto.a     $ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}/lib/libcrypto.a
      COMMAND         ${CMAKE_COMMAND} -E copy            <BINARY_DIR>/decrepit/libdecrepit.a $ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}/lib/libdecrepit.a
)

add_custom_target(trigger-boringssl)
add_dependencies(trigger-boringssl boringssl)
