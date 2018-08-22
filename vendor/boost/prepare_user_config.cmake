if (AIRMAPD_TARGET_PLATFORM MATCHES "android_*")
    set(USER_CONFIG_JAM
        "using clang : android
        :
        ${CMAKE_CXX_COMPILER}
        :
        <cflags>${CMAKE_C_FLAGS}
        <cxxflags>${CMAKE_CXX_FLAGS}
        <archiver>${CMAKE_AR}
        <ranlib>${CMAKE_RANLIB}
        \;
        ")
    file(WRITE ${CMAKE_BINARY_DIR}/user-config.jam ${USER_CONFIG_JAM})
elseif(AIRMAPD_TARGET_PLATFORM MATCHES "ios*")
endif()
