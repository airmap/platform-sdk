//
//  user_agent.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/net/http/user_agent.h>

#include <airmap/util/fmt.h>
#include <airmap/version.h>

std::string airmap::net::http::user_agent() {
  return util::fmt::sprintf("AirMap/%d.%d.%d", Version::current().major, Version::current().minor,
                            Version::current().patch);
}
