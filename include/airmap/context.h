#ifndef AIRMAP_CONTEXT_H_
#define AIRMAP_CONTEXT_H_

#include <airmap/client.h>
#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/logger.h>
#include <airmap/outcome.h>

#include <functional>
#include <unordered_set>

namespace airmap {

/// Context consitutes the point-of-entry for interaction with the classes and interfaces
/// in airmap::*.
class Context : DoNotCopyOrMove {
 public:
  /// ReturnCode enumerates all known return values for a call to run or exec.
  enum class ReturnCode {
    success         = 0,  /// Execution finished successfully
    error           = 1,  /// Execution finished with an error
    already_running = 2   /// Indicates that the context is already executing on another thread
  };

  /// @cond
  using ClientCreateResult   = Outcome<std::shared_ptr<Client>, std::exception_ptr>;
  using ClientCreateCallback = std::function<void(const ClientCreateResult&)>;
  using CreateResult         = Outcome<std::shared_ptr<Context>, std::exception_ptr>;
  using SignalHandler        = std::function<void(int)>;
  using SignalSet            = std::unordered_set<int>;
  /// @endcond

  /// create tries to assemble and return a new Context instance.
  static CreateResult create(const std::shared_ptr<Logger>& logger);

  /// create_client_with_configuration schedules creation of a new client with 'configuration'
  /// and reports results to 'cb'.
  virtual void create_client_with_configuration(const Client::Configuration& configuration,
                                                const ClientCreateCallback& cb) = 0;

  /// exec hands a thread of execution to a Context instance, monitoring
  /// the signals present in 'signal_set' and dispatching incoming signals
  /// to the registered handlers.
  ///
  /// Implementations are expected to block the current thread until
  /// either an error occured or the user explicitly requests a Context
  /// instance to stop.
  virtual ReturnCode exec(const SignalSet& signal_set, const SignalHandler& handler) = 0;

  /// run hands a thread of execution to the context.
  ///
  /// Implementations are expected to block the current thread until
  /// either an error occured or the user explicitly requests a Context
  /// instance to stop.
  virtual ReturnCode run() = 0;

  /// stop requests an instance to shut down its operation and return from
  /// run.
  virtual void stop(ReturnCode rc = ReturnCode::success) = 0;

  /// dispatch executes 'task' on the thread running this Context instance.
  virtual void dispatch(const std::function<void()>& task) = 0;

  /// schedule_in schedules execution of 'functor' in 'wait_for' [us].
  virtual void schedule_in(const Microseconds& wait_for, const std::function<void()>& functor) = 0;

 protected:
  /// @cond
  Context() = default;
  /// @endcond
};

}  // namespace airmap

#endif  // AIRMAP_CONTEXT_H_
