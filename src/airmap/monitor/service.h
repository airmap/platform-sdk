#ifndef AIRMAP_MONITOR_SERVICE_H_
#define AIRMAP_MONITOR_SERVICE_H_

#include <airmap/grpc/server/service.h>

#include <airmap/aircrafts.h>
#include <airmap/client.h>
#include <airmap/traffic.h>

#include "grpc/airmap/monitor/monitor.grpc.pb.h"

namespace airmap {
namespace monitor {

/// Service exposes the daemon via gRPC.
///
/// An instance subscribes to incoming traffic updates
/// and forwards the updates to subscribers connected via gRPC.
class Service : public airmap::grpc::server::Service {
 public:
  /// Service initializes a new instance with 'traffic_monitor'
  explicit Service(const std::shared_ptr<Traffic::Monitor>& traffic_monitor);

  // From airmap::grpc::server::Service.
  ::grpc::Service& instance() override;
  void start(::grpc::ServerCompletionQueue& completion_queue) override;

 private:
  // ConnectToUpdates models the state of a single invocation of
  // the method 'ConnectToUpdates'.
  class ConnectToUpdates : public MethodInvocation {
   public:
    using Parameters = ::grpc::airmap::monitor::ConnectToUpdatesParameters;
    using Result     = ::grpc::airmap::monitor::Update;
    using Responder  = ::grpc::ServerAsyncWriter<Result>;

    // start_listening sets up a new ConnectToUpdates and enqueues it
    // for handling incoming requests.
    static void start_listening(::grpc::ServerCompletionQueue* completion_qeueu,
                                ::grpc::airmap::monitor::Monitor::AsyncService* async_monitor,
                                const std::shared_ptr<Traffic::Monitor>& traffic_monitor);

    // write sends out 'update'.
    void write(const ::grpc::airmap::monitor::Update& update);

    // From MethodInvocation
    void proceed(bool result) override;

   private:
    // State enumerates all known states of the invocation.
    enum class State { ready = 0, streaming = 1, finished = 2 };

    // Subscriber handles incoming traffic updates and bridges them
    // over to a Responder instance.
    //
    // Please note that the lifetime of a Subscriber instance has to be < than the lifetime
    // of the responder instance.
    class Subscriber : public Traffic::Monitor::Subscriber {
     public:
      // Subscriber initializes a new instance with 'responder'.
      explicit Subscriber(ConnectToUpdates* invocation);

      // From Traffic::Monitor::Subscriber
      void handle_update(airmap::Traffic::Update::Type type,
                         const std::vector<airmap::Traffic::Update>& update) override;

     private:
      ConnectToUpdates* invocation_;
    };

    ConnectToUpdates(::grpc::ServerCompletionQueue* completion_queue,
                     ::grpc::airmap::monitor::Monitor::AsyncService* async_monitor,
                     const std::shared_ptr<Traffic::Monitor>& traffic_monitor);

    State state_{State::ready};
    ::grpc::ServerCompletionQueue* completion_queue_;
    ::grpc::airmap::monitor::Monitor::AsyncService* async_monitor_;
    std::shared_ptr<Traffic::Monitor> traffic_monitor_;
    std::shared_ptr<Subscriber> traffic_monitor_subscriber_;
    ::grpc::ServerContext server_context_;
    Parameters parameters_;
    Responder responder_;
  };

  std::shared_ptr<Traffic::Monitor> traffic_monitor_;
  ::grpc::airmap::monitor::Monitor::AsyncService async_monitor_;
};

}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_MONITOR_MONITOR_H_
