cmake_minimum_required(VERSION 3.6)

# This cmake setup builds boost in a way suitable for linking with
# the AirMap platform SDK. 
#
# We are aiming to support the following platforms/architectures:
#
#   * linux/windows/android/macOS x86(64)
#   * linux/android/ios arm(64)
#
# To this end, we translate CMake variables and cross-compilation facilities
# to boost.build.
project(external-boost C CXX)

include(ExternalProject)

set(
  AIRMAP_BOOST_COMPONENTS
  "--with-date_time --with-filesystem --with-log --with-program_options --with-test --with-system --with-thread"
)

if (${ANDROID})
  message(STATUS "Enabling Android build for Boost - ${ANDROID_ABI}")

  string(REPLACE " " " <cflags>"    AIRMAP_BOOST_ANDROID_STANDARD_C_FLAGS       "<cflags>${CMAKE_C_FLAGS}")
  string(REPLACE " " " <cxxflags>"  AIRMAP_BOOST_ANDROID_STANDARD_CXX_FLAGS     "<cxxflags>${CMAKE_CXX_FLAGS}")
  string(REPLACE " " " <linkflags>" AIRMAP_BOOST_ANDROID_STANDARD_CXX_LIBRARIES "<linkflags>${CMAKE_CXX_STANDARD_LIBRARIES}")

  foreach (isystem IN LISTS CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES)
    set(AIRMAP_BOOST_ANDROID_STANDARD_INCLUDE_DIRECTORIES "<cxxflags>-isystem <cxxflags>${isystem} ${AIRMAP_BOOST_ANDROID_STANDARD_INCLUDE_DIRECTORIES}")
  endforeach ()

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/android/user-config.jam.in 
    ${CMAKE_CURRENT_BINARY_DIR}/android/user-config.jam
  )

  set(AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG --user-config=${CMAKE_CURRENT_BINARY_DIR}/android/user-config.jam)
  set(AIRMAP_BOOST_B2_COMMAND_FLAGS_TARGET_OS target-os=android)
  set(AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET toolset=clang-android)
  
  if (${ANDROID_ABI} STREQUAL "armeabi")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=arm)
  elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=arm)
  elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=arm)
  elseif (${ANDROID_ABI} STREQUAL "x86")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=x86)
  elseif (${ANDROID_ABI} STREQUAL "x86_64")
    set(AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE architecture=ia64)
  endif ()
else ()
  set(AIRMAP_BOOST_CXX_FLAGS "cxxflags=-fPIC")
endif ()

if (AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG)
  message(STATUS "  user config: ${AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG}")
else ()
  message(STATUS "  user config: not set")
endif()

if (AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE)
  message(STATUS "  arch:        ${AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE}")
else ()
  message(STATUS "  arch:        not set")
endif ()

if (AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET) 
  message(STATUS "  toolset:     ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET}")
else ()
  message(STATUS "  toolset:     not set")
endif ()

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
  URL https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.bz2
  URL_HASH SHA256=59c9b274bc451cf91a9ba1dd2c7fdcaf5d60b1b3aa83f2c9fa143417cc660722
  BUILD_IN_SOURCE 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ${AIRMAP_BOOST_BOOTSTRAP_COMMAND}
  BUILD_COMMAND
    ${AIRMAP_BOOST_B2_COMMAND}
      -d +2
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG}
      --with-date_time 
      --with-filesystem
      --with-log 
      --with-program_options 
      --with-system 
      --with-test 
      --with-thread
      --disable-icu 
      --prefix=$ENV{AIRMAP_BOOST_OUTPUT_PATH}
      define=BOOST_LOG_WITHOUT_IPC
      cxxflags=-fvisibility=hidden
      cxxflags=-fvisibility-inlines-hidden
      ${AIRMAP_BOOST_CXX_FLAGS} 
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET} 
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE} 
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TARGET_OS} 
      threading=multi link=static variant=release
  INSTALL_COMMAND
    ${AIRMAP_BOOST_B2_COMMAND} 
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_USER_CONFIG} 
      --with-date_time 
      --with-filesystem 
      --with-log 
      --with-program_options 
      --with-system 
      --with-test 
      --with-thread
      --disable-icu 
      --prefix=$ENV{AIRMAP_BOOST_OUTPUT_PATH}
      define=BOOST_LOG_WITHOUT_IPC
      cxxflags=-fvisibility=hidden
      cxxflags=-fvisibility-inlines-hidden
      ${AIRMAP_BOOST_CXX_FLAGS} 
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TOOLSET} 
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_ARCHITECTURE} 
      ${AIRMAP_BOOST_B2_COMMAND_FLAGS_TARGET_OS} 
      threading=multi link=static variant=release install
)
