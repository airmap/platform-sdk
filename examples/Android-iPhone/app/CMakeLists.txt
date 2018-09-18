cmake_minimum_required(VERSION 3.4.1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(FetchContent)

FetchContent_Declare(
    airmapd
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
    BUILD_IN_SOURCE 0
)

FetchContent_GetProperties(airmapd)
if(NOT airmapd_POPULATED)
    FetchContent_Populate(airmapd)
    add_subdirectory(${airmapd_SOURCE_DIR} ${airmapd_BINARY_DIR})
endif()

add_library( native-lib SHARED src/main/cpp/jni_interface.cc )
find_library( log-lib log )
find_library( android-lib android )
add_library( airmap-import STATIC IMPORTED )
set_target_properties(airmap-import
        PROPERTIES
        IMPORTED_LOCATION "${airmapd_BINARY_DIR}/libairmap-c++.a"
        INTERFACE_INCLUDE_DIRECTORIES "${airmapd_SOURCE_DIR}/include"
        INTERFACE_INCLUDE_DIRECTORIES "${airmapd_SOURCE_DIR}/src"
        INTERFACE_INCLUDE_DIRECTORIES "${boost_SOURCE_DIR}/src"
        )
add_dependencies(airmap-import airmapd)
target_link_libraries( native-lib ${log-lib} ${android-lib} airmap-import  )