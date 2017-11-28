include(ExternalProject)

function (build_protobuf)
    set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_protobuf)

    #mktemp dir in build tree
    file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

    #generate false dependency project
    set(CMAKE_LIST_CONTENT "
        cmake_minimum_required(VERSION 2.8)

        include(ExternalProject)
        ExternalProject_add(
            protobuf
            GIT_REPOSITORY https://github.com/google/protobuf.git
            GIT_TAG v3.5.0
            PREFIX protobuf
            SOURCE_SUBDIR cmake
            CMAKE_ARGS 
                -DCMAKE_INSTALL_PREFIX=${AIRMAP_PROTOBUF_OUTPUT_PATH}
                -Dprotobuf_BUILD_TESTS=OFF
                -Dprotobuf_BUILD_SHARED_LIBS=OFF
                -Dprotobuf_WITH_ZLIB=OFF
                -Dprotobuf_MODULE_COMPATIBLE=ON
            UPDATE_COMMAND \"\"
        )

        add_custom_target(trigger_protobuf)
        add_dependencies(trigger_protobuf protobuf)"
    )
    
    file(WRITE ${trigger_build_dir}/CMakeLists.txt "${CMAKE_LIST_CONTENT}")

    execute_process(COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} ..
        WORKING_DIRECTORY ${trigger_build_dir}/build
    )
    
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${trigger_build_dir}/build
    )
endfunction()

function (build_cares)
    set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_cares)

    #mktemp dir in build tree
    file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

    #generate false dependency project
    set(CMAKE_LIST_CONTENT "
        cmake_minimum_required(VERSION 2.8)
        include(ExternalProject)
        ExternalProject_add(
            cares
            GIT_REPOSITORY https://github.com/c-ares/c-ares.git
            GIT_TAG cares-1_13_0
            PREFIX cares
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${AIRMAP_CARES_OUTPUT_PATH}
                -DCARES_STATIC=ON
                -DCARES_DYNAMIC=OFF
                -DCARES_STATIC_PIC=ON
            UPDATE_COMMAND \"\"
        )

        add_custom_target(trigger_cares)
        add_dependencies(trigger_cares cares)"
    )
    
    file(WRITE ${trigger_build_dir}/CMakeLists.txt "${CMAKE_LIST_CONTENT}")

    execute_process(COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} ..
        WORKING_DIRECTORY ${trigger_build_dir}/build
    )
    
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${trigger_build_dir}/build
    )
endfunction()

function (build_grpc)
    set(trigger_build_dir ${CMAKE_BINARY_DIR}/force_grpc)

    #mktemp dir in build tree
    file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

    #generate false dependency project
    set(CMAKE_LIST_CONTENT "
        cmake_minimum_required(VERSION 2.8)
        set(CMAKE_PREFIX_PATH ${AIRMAP_PROTOBUF_OUTPUT_PATH})
        include(${AIRMAP_PROTOBUF_OUTPUT_PATH}/lib/cmake/protobuf/protobuf-config.cmake)
        include(ExternalProject)
        ExternalProject_add(
            grpc
            GIT_REPOSITORY https://github.com/grpc/grpc.git
            GIT_TAG v1.7.2
            PREFIX grpc
            CMAKE_ARGS
                -DCMAKE_PREFIX_PATH=${AIRMAP_PROTOBUF_OUTPUT_PATH};${AIRMAP_CARES_OUTPUT_PATH}
                -DCMAKE_INSTALL_PREFIX=${AIRMAP_GRPC_OUTPUT_PATH}
                -DgRPC_BUILD_TESTS=OFF
                -DgRPC_ZLIB_PROVIDER=package
                -DgRPC_CARES_PROVIDER=package
                -DgRPC_SSL_PROVIDER=package
                -DgRPC_PROTOBUF_PROVIDER=package
                -DgRPC_PROTOBUF_PACKAGE_TYPE=CONFIG
                -DgRPC_INSTALL_default=ON
            UPDATE_COMMAND \"\"
        )

        add_custom_target(trigger_grpc)
        add_dependencies(trigger_grpc grpc)"
    )
    
    file(WRITE ${trigger_build_dir}/CMakeLists.txt "${CMAKE_LIST_CONTENT}")

    execute_process(COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} ..
        WORKING_DIRECTORY ${trigger_build_dir}/build
    )
    
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${trigger_build_dir}/build
    )
endfunction()

build_protobuf()
build_cares()
build_grpc()