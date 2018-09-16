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
//  init.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_GRPC_INIT_H_
#define AIRMAP_GRPC_INIT_H_

namespace airmap {
namespace grpc {

// init initializes the grpc runtime and sets up logging:
//   - if neither GRPC_TRACE nor GRPC_VERBOSITY is set in the env,
//     we silence all grpc logs. With that, we prevent grpc from
//     spamming our logs in production scenarios but we reserve the
//     possibility to debug in case of issues.
bool init();

}  // namespace grpc
}  // namespace airmap

#endif  // AIRMAP_GRPC_INIT_H_