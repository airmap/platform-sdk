#include <airmap/grpc/server/executor.h>

#include <fmt/printf.h>

airmap::grpc::server::Executor::Executor(const Configuration& c) : services_{c.services} {
  grpc_init();
  ::grpc::ServerBuilder builder;

  builder.AddListeningPort(c.endpoint, c.credentials);

  // Register all known services on this Executor instance.
  for (auto service : services_)
    builder.RegisterService(&service->instance());

  // Get hold of the completion queue used for the asynchronous communication
  // with the gRPC runtime.
  server_completion_queue_ = builder.AddCompletionQueue();

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
    if (!ok)
      break;
    if (auto method_invocation = static_cast<Service::MethodInvocation*>(tag))
      method_invocation->proceed();
  }
}

void airmap::grpc::server::Executor::stop() {
  server_->Shutdown();
  server_completion_queue_->Shutdown();
}
