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
//  service.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_GRPC_SERVER_SERVICE_H_
#define AIRMAP_GRPC_SERVER_SERVICE_H_

#include <airmap/do_not_copy_or_move.h>

#include <grpc++/grpc++.h>

namespace airmap {
namespace grpc {
namespace server {

class Service : DoNotCopyOrMove {
 public:
  // instance returns a ::grpc::Service instance with its lifetime bound
  // to the lifetime of this Service instance.
  virtual ::grpc::Service& instance() = 0;

  // start requests the service to start serving requests on 'completion_queue'.
  virtual void start(::grpc::ServerCompletionQueue& completion_queue) = 0;

 protected:
  Service() = default;
};

}  // namespace server
}  // namespace grpc
}  // namespace airmap

#endif  // AIRMAP_GRPC_SERVER_SERVICE_H_
