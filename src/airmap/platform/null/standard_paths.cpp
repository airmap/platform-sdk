//
//  standard_paths.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/platform/null/standard_paths.h>

airmap::Optional<airmap::platform::Path> airmap::platform::null::StandardPaths::path(Scope scope, Location location) {
  return initial_path();
}
