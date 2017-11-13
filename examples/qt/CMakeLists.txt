qt5_wrap_cpp(mocs client.h)
add_executable(client client.cpp ${mocs})
set_property(TARGET client PROPERTY CXX_STANDARD 14)
target_link_libraries(client airmap-qt)

if (AIRMAP_ENABLE_NETWORK_TESTS)
  add_test(examples-qt-client-test client)
endif()
