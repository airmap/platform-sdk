//
//  interface.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/platform/interface.h>

#if defined(AIRMAP_PLATFORM_LINUX)
#include <airmap/platform/linux/interface.h>
std::shared_ptr<airmap::platform::Interface> airmap::platform::create_interface() {
  return std::make_shared<airmap::platform::linux_::Interface>();
}
#endif  // AIRMAP_PLATFORM_LINUX

#if defined(AIRMAP_PLATFORM_NULL)
#include <airmap/platform/null/interface.h>
std::shared_ptr<airmap::platform::Interface> airmap::platform::create_interface() {
  return std::make_shared<airmap::platform::null::Interface>();
}
#endif  // AIRMAP_PLATFORM_NULL
