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

class Traffic : DoNotCopyOrMove {
 public:
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

  class Monitor : DoNotCopyOrMove {
   public:
    struct Params {
      std::string flight_id;
      std::string authorization;
    };

    using Result   = Outcome<std::shared_ptr<Monitor>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;

    class Subscriber {
     public:
      virtual void handle_update(const std::vector<Update>& update) = 0;

     protected:
      Subscriber() = default;
    };

    class FunctionalSubscriber : public Subscriber {
     public:
      explicit FunctionalSubscriber(const std::function<void(const std::vector<Update>&)>& f);
      void handle_update(const std::vector<Update>& update) override;

     private:
      std::function<void(const std::vector<Update>&)> f_;
    };

    class LoggingSubscriber : public Subscriber {
     public:
      explicit LoggingSubscriber(const char* component, const std::shared_ptr<Logger>& logger);
      void handle_update(const std::vector<Update>& update) override;

     private:
      const char* component_;
      std::shared_ptr<Logger> logger_;
    };

    virtual void subscribe(const std::shared_ptr<Subscriber>& subscriber)   = 0;
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
