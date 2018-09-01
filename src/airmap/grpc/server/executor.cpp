//
//  executor.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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
