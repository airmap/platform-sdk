#ifndef AIRMAP_MONITOR_MONITOR_H_
#define AIRMAP_MONITOR_MONITOR_H_

#include <airmap/grpc/server/service.h>

#include <airmap/aircrafts.h>
#include <airmap/client.h>
#include <airmap/traffic.h>

#include "grpc/airmap/monitor/monitor.grpc.pb.h"

namespace airmap {
namespace monitor {

class Service : public airmap::grpc::server::Service {
 public:
  Service(const std::shared_ptr<Traffic::Monitor>& traffic_monitor);

  // From airmap::grpc::server::Service.
  ::grpc::Service& instance() override;
  void start(::grpc::ServerCompletionQueue& completion_queue) override;

 private:
  class ConnectToUpdates : public StatefulMethodInvocation {
   public:
    using Parameters = ::grpc::airmap::monitor::ConnectToUpdatesParameters;
    using Result     = ::grpc::airmap::monitor::Update;
    using Responder  = ::grpc::ServerAsyncWriter<Result>;

    static void start_listening(::grpc::ServerCompletionQueue* completion_qeueu,
                                ::grpc::airmap::monitor::Monitor::AsyncService* async_monitor,
                                const std::shared_ptr<Traffic::Monitor>& traffic_monitor);

    // From MethodInvocation
    void proceed() override;

   private:
    // Subscriber handles incoming traffic updates and bridges them
    // over to a Responder instance.
    //
    // Please note that the lifetime of a Subscriber instance has to be < than the lifetime
    // of the responder instance.
    class Subscriber : public Traffic::Monitor::Subscriber {
     public:
      // Subscriber initializes a new instance with 'responder'.
      explicit Subscriber(Responder* responder);

      // From Traffic::Monitor::Subscriber
      void handle_update(airmap::Traffic::Update::Type type,
                         const std::vector<airmap::Traffic::Update>& update) override;

     private:
      Responder* responder_;
    };

    ConnectToUpdates(::grpc::ServerCompletionQueue* completion_queue,
                     ::grpc::airmap::monitor::Monitor::AsyncService* async_monitor,
                     const std::shared_ptr<Traffic::Monitor>& traffic_monitor);

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
