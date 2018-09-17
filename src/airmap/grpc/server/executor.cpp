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
#include <airmap/grpc/server/executor.h>

#include <airmap/grpc/method_invocation.h>

airmap::grpc::server::Executor::Executor(const Configuration& c) : context_{c.context}, services_{c.services} {
  grpc_init();
  ::grpc::ServerBuilder builder;

  builder.AddListeningPort(c.endpoint, c.credentials);

  // Register all known services on this Executor instance.
  for (auto service : services_)
    builder.RegisterService(&service->instance());

  // Get hold of the completion queue used for the asynchronous communication
  // with the gRPC runtime.
  server_completion_queue_ = builder.AddCompletionQueue(true);

  // Finally assemble the server.
  server_ = builder.BuildAndStart();

  // Request all services to start handling requests.
  for (auto service : services_)
    service->start(*server_completion_queue_);
}

airmap::grpc::server::Executor::~Executor() {
  stop();
}

void airmap::grpc::server::Executor::run() {
  bool ok   = false;
  void* tag = nullptr;

  while (server_completion_queue_->Next(&tag, &ok)) {
    if (auto method_invocation = static_cast<MethodInvocation*>(tag)) {
      context_->dispatch([method_invocation, ok]() { method_invocation->proceed(ok); });
    }

    if (!ok)
      break;
  }
}

void airmap::grpc::server::Executor::stop() {
  server_->Shutdown();
  server_completion_queue_->Shutdown();
}
