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