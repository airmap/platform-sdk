add_library(
  airmap-cpp SHARED

  $<TARGET_OBJECTS:airmap-client>
)

set_target_properties(
  airmap-cpp PROPERTIES

  CXX_STANDARD      14
  SOVERSION         "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
  VERSION           "${PROJECT_VERSION_MAJOR}"
)

if (AIRMAP_ENABLE_GRPC)
  list(
    APPEND CONDITIONAL_LIBRARIES
    airmap-codec-grpc
    airmap-grpc
    airmap-monitor
  )
endif ()

target_link_libraries(
  airmap-cpp
  airmap-mavlink

  ${CONDITIONAL_LIBRARIES}

  ${Boost_LIBRARIES}

  OpenSSL::Crypto
  OpenSSL::SSL
  ${WE_NEED_BORINGSSLS_LIB_DECREPIT}

  protobuf::libprotobuf
)

install(
  TARGETS airmap-cpp
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)