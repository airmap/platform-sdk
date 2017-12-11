cmake_minimum_required(VERSION 2.8)

project(external-boost)

include(ExternalProject)

if(UNIX)
  set(AIRMAP_BOOST_BOOTSTRAP_COMMAND ./bootstrap.sh)
  set(AIRMAP_BOOST_B2_COMMAND ./b2)
else()
  if(WIN32)
    set(AIRMAP_BOOST_BOOTSTRAP_COMMAND bootstrap.bat)
    set(AIRMAP_BOOST_B2_COMMAND b2.exe)
  endif()
endif()

ExternalProject_Add(boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.65.1
  BUILD_IN_SOURCE 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ${AIRMAP_BOOST_BOOTSTRAP_COMMAND}
  BUILD_COMMAND ${AIRMAP_BOOST_B2_COMMAND} install
    --with-date_time
    --with-filesystem
    --with-program_options
    --with-system
    --with-test
    --disable-icu
    --prefix=$ENV{AIRMAP_BOOST_OUTPUT_PATH}
    cxxflags=-fPIC
    threading=multi
    link=static
    variant=release
    -j4
  INSTALL_COMMAND ""
  INSTALL_DIR ${CMAKE_BINARY_DIR}/INSTALL
)