// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <airmap/grpc/init.h>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

namespace {

void drop(gpr_log_func_args*) {
  // Empty on purpose.
}

}  // namespace

bool airmap::grpc::init() {
  static const bool initialized = []() {
    if (!std::getenv("GRPC_TRACE") && !std::getenv("GRPC_VERBOSITY"))
      gpr_set_log_function(drop);
    grpc_init();
    return true;
  }();

  return initialized;
}
