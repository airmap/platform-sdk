cmake_minimum_required(VERSION 3.5)

project(external-boost C CXX)

include(ExternalProject)
include(ProcessorCount)

find_package(Git QUIET REQUIRED)

ProcessorCount(NUM_CPUS)

#if(UNIX)
#  set(AIRMAP_BOOST_BOOTSTRAP_COMMAND ./bootstrap.sh)
#  set(AIRMAP_BOOST_B2_COMMAND ./b2)
#  set(AIRMAP_BOOST_CXX_FLAGS "cxxflags=-std=c++14 -fPIC")
#else()
#  if(WIN32)
#    set(AIRMAP_BOOST_BOOTSTRAP_COMMAND bootstrap.bat)
#    set(AIRMAP_BOOST_B2_COMMAND b2.exe)
#  endif()
#endif()

#message(STATUS ${ANDROID_ABI})
#message(STATUS ${CMAKE_C_COMPILER})
#message(STATUS ${CMAKE_C_FLAGS})
#message(STATUS ${CMAKE_CXX_COMPILER})
#message(STATUS ${CMAKE_CXX_FLAGS})
#message(STATUS ${CMAKE_AR})
#message(STATUS ${CMAKE_RANLIB})


#include(prepare_user_config.cmake)

ExternalProject_Add(boost
  URL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.bz2
  URL_HASH SHA256=5721818253e6a0989583192f96782c4a98eb6204965316df9f5ad75819225ca9
  DOWNLOAD_DIR boost-prefix
  SOURCE_DIR boost-prefix/src/boost_1_66_0
  BUILD_IN_SOURCE 1
  PATCH_COMMAND
    patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/patches/libressl.patch
  CONFIGURE_COMMAND ""
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/boost_ios.sh ../../boost_ios.sh
  BUILD_COMMAND PREFIXDIR=$ENV{AIRMAP_BOOST_OUTPUT_PATH} ../../boost_ios.sh -ios --no-framework
  #CONFIGURE_COMMAND ${AIRMAP_BOOST_BOOTSTRAP_COMMAND}
  #BUILD_COMMAND
  #  ${AIRMAP_BOOST_B2_COMMAND} --with-date_time --with-filesystem --with-log --with-program_options --with-system --with-test --with-thread --disable-icu --prefix=$ENV{AIRMAP_BOOST_OUTPUT_PATH} ${AIRMAP_BOOST_CXX_FLAGS} threading=multi link=static variant=release -j${NUM_CPUS}
  #INSTALL_COMMAND
  #  ${AIRMAP_BOOST_B2_COMMAND} --with-date_time --with-filesystem --with-log --with-program_options --with-system --with-test --with-thread --disable-icu --prefix=$ENV{AIRMAP_BOOST_OUTPUT_PATH} ${AIRMAP_BOOST_CXX_FLAGS} threading=multi link=static variant=release install
)
