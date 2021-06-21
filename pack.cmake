set(CPACK_PACKAGE_NAME "airmap-platform-sdk")
set(CPACK_PACKAGE_VENDOR "AirMap")
set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
if (ANDROID)
endif (ANDROID)
if (UNIX)
    find_file (DEBIAN_FOUND debian_version debconf.conf PATHS /etc)
    if (DEBIAN_FOUND)
        set(CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "AirMap, Inc.")
    endif (DEBIAN_FOUND)
endif (UNIX)
include(CPack)
