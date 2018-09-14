if (AIRMAP_ENABLE_GRPC)
  add_subdirectory(monitor)
endif ()

if (AIRMAP_ENABLE_QT)
  add_subdirectory(qt)
endif ()
