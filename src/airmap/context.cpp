//
//  context.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/context.h>

#include <airmap/boost/context.h>

airmap::Context::CreateResult airmap::Context::create(const std::shared_ptr<Logger>& logger) {
  return CreateResult{boost::Context::create(logger)};
}
