/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  version.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/version.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

cmd::Version::Version()
    : cli::CommandWithFlagsAndAction{"version", "prints version information", "prints version information"} {
  action([this](const cli::Command::Context& ctxt) {
    ctxt.cout << ::airmap::Version::current().major << "." << ::airmap::Version::current().minor << "."
              << ::airmap::Version::current().patch << " " << ::airmap::Version::current().git_revision << " "
              << ::airmap::Version::current().build_timestamp << std::endl;
    return 0;
  });
}
