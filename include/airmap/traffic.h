#ifndef AIRMAP_TRAFFIC_H_
#define AIRMAP_TRAFFIC_H_

#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/logger.h>
#include <airmap/outcome.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace airmap {

/// Traffic provides access to the AirMap situational awareness
/// and traffic alerts.
class Traffic : DoNotCopyOrMove {
 public:
  /// Update bundles together information about aerial traffic
  /// relevant to a UAV flight.
  struct Update {
    std::string id;
    std::string aircraft_id;
    std::string direction;
    double latitude;
    double longitude;
    double altitude;
    double ground_speed;
    double heading;

    DateTime recorded;
    DateTime timestamp;
  };

  /// Monitor models handling of individual subscribers
  /// to per-flight alerts and awareness notices.
  class Monitor : DoNotCopyOrMove {
   public:
    struct Params {
      std::string flight_id;
      std::string authorization;
    };

    using Result   = Outcome<std::shared_ptr<Monitor>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;

    /// Subscriber abstracts handling of batches of Update instances.
    class Subscriber {
     public:
      /// handle_update is invoked when a new batch of Update instances
      /// is available.
      virtual void handle_update(const std::vector<Update>& update) = 0;

     protected:
      Subscriber() = default;
    };

    /// FunctionalSubscriber is a convenience class that dispatches
    /// to a function 'f' for handling batches of Update instances.
    class FunctionalSubscriber : public Subscriber {
     public:
      /// FunctionalSubscriber initializes a new instance with 'f'.
      explicit FunctionalSubscriber(const std::function<void(const std::vector<Update>&)>& f);
      // From subscriber
      void handle_update(const std::vector<Update>& update) override;

     private:
      std::function<void(const std::vector<Update>&)> f_;
    };

    /// LoggingSubscriber is a convenience class that logs incoming batches
    /// of Update instances.
    class LoggingSubscriber : public Subscriber {
     public:
      /// LoggingSubscriber initializes an instance with 'component', feeding
      /// log entries to 'logger'. Please note that no change of ownership takes
      /// place for 'component' and the lifetime of component has to exceed the
      /// lifetime of a LoggingSubscriber instance.
      explicit LoggingSubscriber(const char* component, const std::shared_ptr<Logger>& logger);

      // From Subscriber
      void handle_update(const std::vector<Update>& update) override;

     private:
      const char* component_;
      std::shared_ptr<Logger> logger_;
    };

    /// subscribe registers 'subscriber' such that subsequent batches of
    /// Update instances are delivered to 'subscriber'.
    virtual void subscribe(const std::shared_ptr<Subscriber>& subscriber) = 0;

    /// unsubscribe unregisters 'subscriber'.
    virtual void unsubscribe(const std::shared_ptr<Subscriber>& subscriber) = 0;

   protected:
    Monitor() = default;
  };

  /// monitor subscribes the user and flight described in 'params' to
  /// the AirMap traffic services and reports the result to 'cb'.
  virtual void monitor(const Monitor::Params& params, const Monitor::Callback& cb) = 0;

 protected:
  Traffic() = default;
};

}  // namespace airmap

#endif  // AIRMAP_TRAFFIC_H_
