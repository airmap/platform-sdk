#include <airmap/grpc/init.h>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

namespace {

void drop(gpr_log_func_args*) {
  // Empty on purpose.
}

}  // namespace

bool airmap::grpc::init() {
  static const bool initialized = []() {
    if (!std::getenv("GRPC_TRACE") && !std::getenv("GRPC_VERBOSITY"))
      gpr_set_log_function(drop);
    grpc_init();
    return true;
  }();

  return initialized;
}