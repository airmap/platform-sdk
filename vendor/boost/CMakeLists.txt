cmake_minimum_required(VERSION 3.5)

project(external-boost C CXX)

include(ExternalProject)
include(ProcessorCount)

find_package(Git QUIET REQUIRED)

ProcessorCount(NUM_CPUS)

if(UNIX)
  set(AIRMAP_BOOST_BOOTSTRAP_COMMAND ./bootstrap.sh)
  set(AIRMAP_BOOST_B2_COMMAND ./b2)
  set(AIRMAP_BOOST_CXX_FLAGS "cxxflags=-std=c++14 -fPIC")
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
  PATCH_COMMAND
    ${GIT_EXECUTABLE} apply --directory=libs/asio -p1 ${CMAKE_CURRENT_SOURCE_DIR}/patches/libressl.patch
  CONFIGURE_COMMAND ${AIRMAP_BOOST_BOOTSTRAP_COMMAND}
  BUILD_COMMAND
    ${AIRMAP_BOOST_B2_COMMAND} headers 
    ${AIRMAP_BOOST_B2_COMMAND} --with-date_time --with-filesystem --with-program_options --with-system --with-test --disable-icu --prefix=$ENV{AIRMAP_BOOST_OUTPUT_PATH} ${AIRMAP_BOOST_CXX_FLAGS} threading=multi link=static variant=release -j${NUM_CPUS}
  INSTALL_COMMAND
    ${AIRMAP_BOOST_B2_COMMAND} --with-date_time --with-filesystem --with-program_options --with-system --with-test --disable-icu --prefix=$ENV{AIRMAP_BOOST_OUTPUT_PATH} ${AIRMAP_BOOST_CXX_FLAGS} threading=multi link=static variant=release install
)
