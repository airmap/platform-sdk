include_directories(${CMAKE_CURRENT_SOURCE_DIR} 3rd_party)

configure_file(helper.cpp.in helper.cpp)

add_library(
    airmap-test-helper

    helper.h
    ${CMAKE_CURRENT_BINARY_DIR}/helper.cpp
)

function(airmap_add_test name source)
  add_executable(${name} ${source})
  target_compile_definitions(${name} PRIVATE -DCATCH_CONFIG_MAIN)
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 14)
  target_link_libraries(${name} airmap-client airmap-test-helper ${Boost_LIBRARIES})
  add_test(${name} ${name})
endfunction (airmap_add_test)

airmap_add_test(airspace_test airspace_test.cpp)
airmap_add_test(client_test client_test.cpp)
airmap_add_test(geometry_test geometry_test.cpp)
airmap_add_test(platform_test platform_test.cpp)
airmap_add_test(rest_test rest_test.cpp)
airmap_add_test(token_test token_test.cpp)
# airmap_add_test(telemetry_test telemetry_test.cpp)
