#ifndef AIRMAP_GRPC_SERVER_SERVICE_H_
#define AIRMAP_GRPC_SERVER_SERVICE_H_

#include <airmap/do_not_copy_or_move.h>

#include <grpc++/grpc++.h>

namespace airmap {
namespace grpc {
namespace server {

class Service : DoNotCopyOrMove {
 public:
  // MethodInvocation abstracts handling of incoming method invocations for dispatching
  // on a ::grpc::ServerCompletionQueue.
  class MethodInvocation : DoNotCopyOrMove {
   public:
    // proceed advances the state of the invocation.
    // 'result' indicates the state of the underlying operation, true if
    // the previous operation succeeded, false otherwise.
    virtual void proceed(bool result) = 0;

   protected:
    MethodInvocation() = default;
  };

  // StatefulMethodInvocation helps with tracking the state of a
  // method invocation.
  class StatefulMethodInvocation : public MethodInvocation {
   protected:
    // Status enumerates all possible states of an instance.
    enum class Status {
      ready,    // The instance is ready to serve incoming requests.
      finished  // The instance is finished serviing requests and ready for cleanup.
    };

    Status status_{Status::ready};  // The current status of the invocation.
  };

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
