set(AIRMAP_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE PATH "place test logs in this directory")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 3rd_party)

configure_file(helper.cpp.in helper.cpp)

add_library(
    airmap-test-helper

    helper.h
    ${CMAKE_CURRENT_BINARY_DIR}/helper.cpp
)

function(airmap_add_test name source)
  add_executable(
    ${name}

    $<TARGET_OBJECTS:airmap-client>

    ${source})

  set_property(TARGET ${name} PROPERTY CXX_STANDARD 14)
  target_link_libraries(
    ${name}

    airmap-grpc
    airmap-monitor
    airmap-test-helper

    ${Boost_LIBRARIES}
    ${OPENSSL_LDFLAGS})

  add_test(${name} ${name} --color_output=false --logger=XML,all,${AIRMAP_TEST_OUTPUT_DIRECTORY}/${name}.xml)
endfunction (airmap_add_test)

airmap_add_test(airspace_test airspace_test.cpp)
airmap_add_test(cli_test cli_test.cpp)
airmap_add_test(client_test client_test.cpp)
airmap_add_test(credentials_test credentials_test.cpp)
# airmap_add_test(daemon_test daemon_test.cpp)
airmap_add_test(datetime_test datetime_test.cpp)
airmap_add_test(error_test error_test.cpp)
airmap_add_test(geometry_test geometry_test.cpp)
airmap_add_test(platform_test platform_test.cpp)
airmap_add_test(rest_test rest_test.cpp)
airmap_add_test(token_test token_test.cpp)

airmap_add_test(issue_38_test issue_38_test.cpp)
# airmap_add_test(telemetry_test telemetry_test.cpp)

if (AIRMAP_ENABLE_NETWORK_TESTS)
  add_test(
    NAME acceptance.laanc
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/acceptance/laanc.sh)

  set_tests_properties(
    acceptance.laanc
    PROPERTIES
    ENVIRONMENT "AIRMAP_EXECUTABLE=$<TARGET_FILE:airmap>"
    WILL_FAIL "true")

  add_test(
    NAME acceptance.rule_sets
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/acceptance/rule_sets.sh)

  set_tests_properties(
    acceptance.rule_sets
    PROPERTIES
    ENVIRONMENT "AIRMAP_EXECUTABLE=$<TARGET_FILE:airmap>")
endif()
