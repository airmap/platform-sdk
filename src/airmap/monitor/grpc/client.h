#ifndef AIRMAP_MONITOR_GRPC_CLIENT_H_
#define AIRMAP_MONITOR_GRPC_CLIENT_H_

#include <airmap/monitor/client.h>

#include <airmap/context.h>
#include <airmap/grpc/client/executor.h>
#include <airmap/grpc/method_invocation.h>
#include <airmap/util/formatting_logger.h>

#include "grpc/airmap/monitor/monitor.grpc.pb.h"

#include <mutex>
#include <set>
#include <thread>

namespace airmap {
namespace monitor {
namespace grpc {

// Client implements airmap::monitor::Client, reaching out to
// a remote service via gRPC.
class Client : public airmap::monitor::Client {
 public:
  // Client initializes a new instance with 'configuration'.
  explicit Client(const Configuration& configuration, const std::shared_ptr<Context>& context);
  ~Client();
  // From airmap::monitor::Client
  void connect_to_updates(const ConnectToUpdates::Callback& cb) override;

 private:
  using Stub = ::grpc::airmap::monitor::Monitor::Stub;

  class UpdateStreamImpl : public UpdateStream {
   public:
    // write_update delivers 'update' to all receivers.
    void write_update(const Update& update);

    // From UpdateStream
    void subscribe(const std::shared_ptr<Receiver>& receiver) override;
    void unsubscribe(const std::shared_ptr<Receiver>& receiver) override;

   private:
    std::mutex receivers_guard_;
    std::set<std::shared_ptr<Receiver>> receivers_;
  };

  // ConnectToUpdatesInvocation bundles up the state of an invocation
  // of connect_to_updates.
  class ConnectToUpdatesInvocation : public airmap::grpc::MethodInvocation {
   public:
    using Parameters = ::grpc::airmap::monitor::ConnectToUpdatesParameters;
    using Element    = ::grpc::airmap::monitor::Update;
    using Stream     = ::grpc::ClientAsyncReader<::grpc::airmap::monitor::Update>;

    // Start creates and runs a new invocation.
    static void start(const std::shared_ptr<Stub>& stub, ::grpc::CompletionQueue* completion_queue,
                      const ConnectToUpdates::Callback& cb, const std::shared_ptr<Context>& context);

    // ConnectToUpdatesInvocation initializes a new instance with 'completion_queue'.
    explicit ConnectToUpdatesInvocation(const std::shared_ptr<Stub>& stub, ::grpc::CompletionQueue* completion_queue,
                                        const ConnectToUpdates::Callback& cb, const std::shared_ptr<Context>& context);

    // From MethodInvocation
    void proceed(bool result) override;

   private:
    enum class State { connecting, streaming, finished };

    State state_{State::connecting};
    ::grpc::Status status_;
    ::grpc::ClientContext client_context_;
    std::shared_ptr<Stub> stub_;

    ::grpc::CompletionQueue* completion_queue_;
    ConnectToUpdates::Callback cb_;
    std::shared_ptr<Context> context_;

    std::shared_ptr<UpdateStreamImpl> update_stream_;
    Element element_;
    std::unique_ptr<Stream> stream_;
  };

  bool is_grpc_initialized_{airmap::grpc::init()};

  util::FormattingLogger log_;
  std::shared_ptr<Context> context_;

  airmap::grpc::client::Executor executor_;
  std::thread executor_worker_;

  std::shared_ptr<Stub> stub_;
};

}  // namespace grpc
}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_MONITOR_GRPC_CLIENT_H_