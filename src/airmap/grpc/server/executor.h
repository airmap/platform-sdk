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
//  executor.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_GRPC_SERVER_EXECUTOR_H_
#define AIRMAP_GRPC_SERVER_EXECUTOR_H_

#include <airmap/context.h>
#include <airmap/grpc/init.h>
#include <airmap/grpc/server/service.h>

#include <grpc++/grpc++.h>

#include <memory>
#include <vector>

namespace airmap {
namespace grpc {
namespace server {

// Executor creates and runs an asynchronous grpc server.
class Executor {
 public:
  // Configuration bundles up creation-time parameters of an Executor.
  struct Configuration {
    std::shared_ptr<Context> context;                        // Target context.
    std::string endpoint;                                    // The endpoint that the server should listen on.
    std::vector<std::shared_ptr<Service>> services;          // The services that should be registered on the server.
    std::shared_ptr<::grpc::ServerCredentials> credentials;  // The credentials used by the server.
  };

  // Executor initializes an Executor instance with 'configuration'.
  Executor(const Configuration& configuration);
  ~Executor();

  // run executes this Executor instance and blocks until either an error occurs or
  // stop is invoked.
  void run();

  // stop requests this Exector instance to stop its operation.
  void stop();

 private:
  bool is_grpc_initialized_{grpc::init()};
  std::shared_ptr<Context> context_;
  std::vector<std::shared_ptr<Service>> services_;
  std::unique_ptr<::grpc::Server> server_;
  std::unique_ptr<::grpc::ServerCompletionQueue> server_completion_queue_;
};

}  // namespace server
}  // namespace grpc
}  // namespace airmap

#endif  // AIRMAP_GRPC_SERVER_EXECUTOR_H_
