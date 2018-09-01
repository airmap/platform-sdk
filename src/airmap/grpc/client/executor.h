//
//  executor.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_GRPC_CLIENT_EXECUTOR_H_
#define AIRMAP_GRPC_CLIENT_EXECUTOR_H_

#include <airmap/do_not_copy_or_move.h>

#include <airmap/grpc/init.h>

#include <grpc++/grpc++.h>

#include <functional>
#include <memory>

namespace airmap {
namespace grpc {
namespace client {

/// Executor creates and runs an asynchronous grpc server.
class Executor {
 public:
  /// invoke_method invokes the given functor f on the ::grpc::CompletionQueue
  /// instance mananged by this Executor instance.
  void invoke_method(const std::function<void(::grpc::CompletionQueue*)>&);

  /// run executes this Executor instance and blocks until either an error occurs or
  /// stop is invoked.
  void run();

  /// stop requests this Exector instance to stop its operation.
  void stop();

 private:
  bool is_grpc_initialized_{grpc::init()};
  ::grpc::CompletionQueue completion_queue_;
};

}  // namespace client
}  // namespace grpc
}  // namespace airmap

#endif  // AIRMAP_GRPC_CLIENT_EXECUTOR_H_
