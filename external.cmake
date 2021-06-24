set(AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH "${CMAKE_BINARY_DIR}/external")
set(ENV{AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH} "${CMAKE_BINARY_DIR}/external")

function (build_boost)
  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_boost)
  if (EXISTS ${trigger_build_dir})
    return()
  endif()

  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DANDROID_ABI=${ANDROID_ABI}
      -DANDROID_PLATFORM=${ANDROID_PLATFORM}
      -DANDROID_STL=${ANDROID_STL}
      -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      ${CMAKE_SOURCE_DIR}/vendor/boost
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_boringssl)
  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_boringssl)
  if (EXISTS ${trigger_build_dir})
    return()
  endif()

  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DANDROID_ABI=${ANDROID_ABI}
      -DANDROID_PLATFORM=${ANDROID_PLATFORM}
      -DANDROID_STL=${ANDROID_STL}
      -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_FIND_ROOT_PATH:PATH="${CMAKE_FIND_ROOT_PATH}"
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      ${CMAKE_SOURCE_DIR}/vendor/boringssl
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_protobuf)
  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_protobuf)
  if (EXISTS ${trigger_build_dir})
    return()
  endif()

  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DANDROID_ABI=${ANDROID_ABI}
      -DANDROID_PLATFORM=${ANDROID_PLATFORM}
      -DANDROID_STL=${ANDROID_STL}
      -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      ${CMAKE_SOURCE_DIR}/vendor/protobuf
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_cares)
  if (NOT AIRMAP_ENABLE_GRPC)
    message(STATUS "Skipping c-ares build")
    return()
  endif()

  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_cares)
  if (EXISTS ${trigger_build_dir})
    return()
  endif()

  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DANDROID_ABI=${ANDROID_ABI}
      -DANDROID_PLATFORM=${ANDROID_PLATFORM}
      -DANDROID_STL=${ANDROID_STL}
      -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      ${CMAKE_SOURCE_DIR}/vendor/cares
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_grpc)
  if (${CMAKE_CROSSCOMPILING} AND AIRMAP_ENABLE_GRPC)
    message(FATAL_ERROR Cross-compiling with gRPC is not supported)
  endif ()

  if (NOT AIRMAP_ENABLE_GRPC)
    message(STATUS "Skipping gRPC build")
    return()
  endif()

  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_grpc)
  if (EXISTS ${trigger_build_dir})
    return()
  endif()

  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DANDROID_ABI=${ANDROID_ABI}
      -DANDROID_PLATFORM=${ANDROID_PLATFORM}
      -DANDROID_STL=${ANDROID_STL}
      -DANDROID_CPP_FEATURES=${ANDROID_CPP_FEATURES}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
      -DOPENSSL_USE_STATIC_LIBS=${OPENSSL_USE_STATIC_LIBS}
      ${CMAKE_SOURCE_DIR}/vendor/grpc
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  find_file(
    AIRMAP_GRPC_CMAKE_CONFIG gRPCConfig.cmake
    PATHS ${AIRMAP_GRPC_OUTPUT_PATH}/lib/cmake/grpc ${AIRMAP_GRPC_OUTPUT_PATH}/${CMAKE_INSTALL_LIBDIR}/cmake/grpc
    NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH NO_CMAKE_FIND_ROOT_PATH)

  if (NOT AIRMAP_GRPC_CMAKE_CONFIG)
    message(FATAL_ERROR "Failed to find grpc cmake config file")
  endif()

  include(${AIRMAP_GRPC_CMAKE_CONFIG})
endfunction()


set(AIRMAP_BOOST_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}")
set(AIRMAP_BORINGSSL_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}")
set(AIRMAP_CARES_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}")
set(AIRMAP_GRPC_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}")
set(AIRMAP_PROTOBUF_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}")

set(ENV{AIRMAP_BOOST_OUTPUT_PATH}     ${AIRMAP_BOOST_OUTPUT_PATH})
set(ENV{AIRMAP_BORINGSSL_OUTPUT_PATH} ${AIRMAP_BORINGSSL_OUTPUT_PATH})
set(ENV{AIRMAP_CARES_OUTPUT_PATH}     ${AIRMAP_CARES_OUTPUT_PATH})
set(ENV{AIRMAP_GRPC_OUTPUT_PATH}      ${AIRMAP_GRPC_OUTPUT_PATH})
set(ENV{AIRMAP_PROTOBUF_OUTPUT_PATH}  ${AIRMAP_PROTOBUF_OUTPUT_PATH})


list(
  APPEND CMAKE_PREFIX_PATH
  ${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}
)

set(ENV{CMAKE_PREFIX_PATH} "${AIRMAP_BOOST_OUTPUT_PATH}:${AIRMAP_BORINGSSL_OUTPUT_PATH}:${AIRMAP_CARES_OUTPUT_PATH}:${AIRMAP_GRPC_OUTPUT_PATH}:${AIRMAP_PROTOBUF_OUTPUT_PATH}")

set(BOOST_ROOT ${AIRMAP_BOOST_OUTPUT_PATH})
set(Boost_USE_MULTITHREADED ON)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS ON)

# Please note that the order of these invocations is important
build_boost()
build_boringssl()
build_protobuf()
build_cares()
build_grpc()

list(
  APPEND CMAKE_FIND_ROOT_PATH
  ${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}
)

include(GNUInstallDirs)

set(OPENSSL_ROOT_DIR ${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH})
set(OPENSSL_USE_STATIC_LIBS ON)