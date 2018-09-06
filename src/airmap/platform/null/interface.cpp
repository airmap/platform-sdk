//
//  interface.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/platform/null/interface.h>

airmap::platform::StandardPaths& airmap::platform::null::Interface::standard_paths() {
  return standard_paths_;
}
