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
    virtual void proceed() = 0;

   protected:
    MethodInvocation() = default;
  };

  class StatefulMethodInvocation : public MethodInvocation {
   protected:
    enum class Status { ready, finished };
    Status status_{Status::ready};
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
