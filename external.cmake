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
      ${CMAKE_SOURCE_DIR}/cmake/external/protobuf
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
      ${CMAKE_SOURCE_DIR}/cmake/external/cares
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
      ${CMAKE_SOURCE_DIR}/cmake/external/grpc
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${trigger_build_dir}/build
  )
endfunction()

# Please note that the order of these invocations is important
build_protobuf()
build_cares()
build_grpc()