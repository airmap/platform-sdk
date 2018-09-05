add_library(
  airmap-mavlink STATIC

  channel.h
  channel.cpp
  global_position_int.h
  global_position_int.cpp
  heartbeat.h
  heartbeat.cpp
  mission.h
  mission.cpp
  router.h
  router.cpp
  state.h
  state.cpp
  vehicle.h
  vehicle.cpp
  vehicle_tracker.h
  vehicle_tracker.cpp
  boost/serial_channel.h
  boost/serial_channel.cpp
  boost/tcp_channel.h
  boost/tcp_channel.cpp
  boost/tcp_route.h
  boost/tcp_route.cpp
  boost/udp_channel.h
  boost/udp_channel.cpp
)

set_property(
  TARGET airmap-mavlink
  PROPERTY CXX_STANDARD 14)
  
set_property(
  TARGET airmap-mavlink
  PROPERTY COMPILE_OPTIONS "-Wno-pedantic")