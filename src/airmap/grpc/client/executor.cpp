#include <airmap/grpc/client/executor.h>

#include <airmap/grpc/method_invocation.h>

void airmap::grpc::client::Executor::invoke_method(const std::function<void(::grpc::CompletionQueue*)>& f) {
  f(&completion_queue_);
}

void airmap::grpc::client::Executor::run() {
  bool ok   = false;
  void* tag = nullptr;

  while (completion_queue_.Next(&tag, &ok)) {
    if (auto mi = static_cast<MethodInvocation*>(tag)) {
      mi->proceed(ok);
    }
  }
}

void airmap::grpc::client::Executor::stop() {
  completion_queue_.Shutdown();
}