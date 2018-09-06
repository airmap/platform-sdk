function (build_boost)
  if (EXISTS ${AIRMAP_BOOST_OUTPUT_PATH})
    return()
  endif()

  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_boost)
  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DAIRMAPD_TARGET_PLATFORM=${AIRMAPD_TARGET_PLATFORM}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DANDROID_STL=${ANDROID_STL}
      -DANDROID_ABI=${ANDROID_ABI}
      -DANDROID_PLATFORM=${ANDROID_PLATFORM}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      ${CMAKE_SOURCE_DIR}/vendor/boost
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_protobuf)
  if (EXISTS ${AIRMAP_PROTOBUF_OUTPUT_PATH})
    return()
  endif()

  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_protobuf)
  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      ${CMAKE_SOURCE_DIR}/vendor/protobuf
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_boringssl)
  if (EXISTS ${AIRMAP_BORINGSSL_OUTPUT_PATH})
    return()
  endif()
  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_boringssl)
  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      ${CMAKE_SOURCE_DIR}/vendor/boringssl
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_cares)
  if (EXISTS ${AIRMAP_CARES_OUTPUT_PATH})
    return()
  endif()
  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_cares)
  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      ${CMAKE_SOURCE_DIR}/vendor/cares
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

function (build_grpc)
  if (EXISTS ${AIRMAP_GRPC_OUTPUT_PATH})
    return()
  endif()
  set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_grpc)
  file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -G${CMAKE_GENERATOR}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}
      -DIOS_PLATFORM=${IOS_PLATFORM}
      -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
      -DOPENSSL_USE_STATIC_LIBS=${OPENSSL_USE_STATIC_LIBS}
      ${CMAKE_SOURCE_DIR}/vendor/grpc
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

set(AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH "${CMAKE_BINARY_DIR}/external")

set(AIRMAP_BOOST_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/boost")
set(AIRMAP_BORINGSSL_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/boringssl")
set(AIRMAP_CARES_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/cares")
set(AIRMAP_GRPC_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/grpc")
set(AIRMAP_PROTOBUF_OUTPUT_PATH "${AIRMAP_EXTERNAL_DEPENDENCIES_OUTPUT_PATH}/protobuf")

set(ENV{AIRMAP_BOOST_OUTPUT_PATH}      ${AIRMAP_BOOST_OUTPUT_PATH})
set(ENV{AIRMAP_BORINGSSL_OUTPUT_PATH}  ${AIRMAP_BORINGSSL_OUTPUT_PATH})
set(ENV{AIRMAP_CARES_OUTPUT_PATH}      ${AIRMAP_CARES_OUTPUT_PATH})
set(ENV{AIRMAP_GRPC_OUTPUT_PATH}       ${AIRMAP_GRPC_OUTPUT_PATH})
set(ENV{AIRMAP_PROTOBUF_OUTPUT_PATH}   ${AIRMAP_PROTOBUF_OUTPUT_PATH})

set(ENV{CMAKE_PREFIX_PATH} "${AIRMAP_BOOST_OUTPUT_PATH}:${AIRMAP_BORINGSSL_OUTPUT_PATH}:${AIRMAP_CARES_OUTPUT_PATH}:${AIRMAP_GRPC_OUTPUT_PATH}:${AIRMAP_PROTOBUF_OUTPUT_PATH}")

list(
  APPEND CMAKE_PREFIX_PATH

  ${AIRMAP_BOOST_OUTPUT_PATH}
  ${AIRMAP_GRPC_OUTPUT_PATH}
  ${AIRMAP_BORINGSSL_OUTPUT_PATH} 
  ${AIRMAP_CARES_OUTPUT_PATH} 
  ${AIRMAP_PROTOBUF_OUTPUT_PATH}
)

set(BOOST_ROOT ${AIRMAP_BOOST_OUTPUT_PATH})
set(Boost_USE_MULTITHREADED ON)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS OFF)

# Please note that the order of these invocations is important
build_boost()
build_boringssl()
build_protobuf()
build_cares()
build_grpc()

include(GNUInstallDirs)

find_file(
  AIRMAP_PROTOBUF_CMAKE_CONFIG protobuf-config.cmake
  PATHS ${AIRMAP_PROTOBUF_OUTPUT_PATH}/lib/cmake/protobuf ${AIRMAP_PROTOBUF_OUTPUT_PATH}/${CMAKE_INSTALL_LIBDIR}/cmake/protobuf
  NO_DEFAULT_PATH)

if (NOT AIRMAP_PROTOBUF_CMAKE_CONFIG)
  message(FATAL_ERROR "Failed to find protobuf cmake config file")
endif()

include(${AIRMAP_PROTOBUF_CMAKE_CONFIG})

find_file(
  AIRMAP_GRPC_CMAKE_CONFIG gRPCConfig.cmake
  PATHS ${AIRMAP_GRPC_OUTPUT_PATH}/lib/cmake/grpc ${AIRMAP_GRPC_OUTPUT_PATH}/${CMAKE_INSTALL_LIBDIR}/cmake/grpc
  NO_DEFAULT_PATH)

if (NOT AIRMAP_GRPC_CMAKE_CONFIG)
  message(FATAL_ERROR "Failed to find grpc cmake config file")
endif()

include(${AIRMAP_GRPC_CMAKE_CONFIG})
