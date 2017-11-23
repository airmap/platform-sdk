#ifndef AIRMAP_GRPC_METHOD_INVICATION_H_
#define AIRMAP_GRPC_METHOD_INVICATION_H_

#include <airmap/do_not_copy_or_move.h>

namespace airmap {
namespace grpc {

/// MethodInvocation abstracts the state of an individual method invocation.
class MethodInvocation : DoNotCopyOrMove {
 public:
  /// proceed is invoked on every MethodInvocation instance
  /// dequeued by an Executor. If 'result' is true, the previous
  /// operation was successful, false otherwise.
  virtual void proceed(bool result) = 0;

 protected:
  MethodInvocation() = default;
};

}  // namespace grpc
}  // namespace airmap

#endif  // AIRMAP_GRPC_METHOD_INVICATION_H_