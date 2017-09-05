include_directories(3rd_party)

add_executable(airspace_test airspace_test.cpp)
set_property(TARGET airspace_test PROPERTY CXX_STANDARD 14)
target_link_libraries(airspace_test airmap-client)
add_test(airspace_test airspace_test)

add_executable(rest_test rest_test.cpp)
set_property(TARGET rest_test PROPERTY CXX_STANDARD 14)
target_link_libraries(rest_test airmap-client)
add_test(rest_test rest_test)

add_executable(telemetry_test telemetry_test.cpp)
set_property(TARGET telemetry_test PROPERTY CXX_STANDARD 14)
target_link_libraries(telemetry_test airmap-client)
add_test(telemetry_test telemetry_test)
