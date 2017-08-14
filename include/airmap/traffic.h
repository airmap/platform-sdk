#ifndef AIRMAP_TRAFFIC_H_
#define AIRMAP_TRAFFIC_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <functional>
#include <stdexcept>
#include <string>

namespace airmap {

class Traffic : DoNotCopyOrMove {
 public:
  struct TrafficInfo {
    std::string TBD;
  };

  struct Subscribe {
    struct Params {
      std::string flight_id;
      std::string token;
    };

    using Result   = Outcome<TrafficInfo, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  /// TBD
  /// subscribe subscribes the user described in 'params' to
  /// the AirMap traffic services and reports the result to 'cb'.
  virtual void subscribe(const Subscribe::Params& params, const Subscribe::Callback& cb) = 0;

  // connect
  // subscribe
  // disconnect
  // on_message

 protected:
  Traffic() = default;
};

}  // namespace airmap

#endif  // AIRMAP_TRAFFIC_H_
