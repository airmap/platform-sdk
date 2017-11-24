#ifndef AIRMAP_GRPC_METHOD_INVICATION_H_
#define AIRMAP_GRPC_METHOD_INVICATION_H_

#include <airmap/do_not_copy_or_move.h>

#include <iosfwd>

namespace airmap {
namespace grpc {

/// MethodInvocation abstracts the state of an individual method invocation.
class MethodInvocation : DoNotCopyOrMove {
 public:
  // State enumerates all known states of a MethodInvocation instance.
  enum class State { ready = 0, streaming = 1, finished = 2 };

  /// proceed is invoked on every MethodInvocation instance
  /// dequeued by an Executor. If 'result' is true, the previous
  /// operation was successful, false otherwise.
  virtual void proceed(bool result) = 0;

 protected:
  MethodInvocation() = default;
};

/// operator<< inserts 'state' into 'out'.
std::ostream& operator<<(std::ostream& out, MethodInvocation::State state);

}  // namespace grpc
}  // namespace airmap

#endif  // AIRMAP_GRPC_METHOD_INVICATION_H_