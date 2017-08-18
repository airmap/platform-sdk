#ifndef AIRMAP_STATUS_H_
#define AIRMAP_STATUS_H_

#include <airmap/airspace.h>
#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace airmap {

class Status : DoNotCopyOrMove {
 public:
  struct Advisory {
    Airspace airspace;
    std::string color;
  };

  struct Info {
    int max_safe_distance;
    std::string advisory_color;
    std::vector<Advisory> advisories;
  };

  struct GetStatus {
    struct Parameters {
      float latitude;
      float longitude;
      Airspace::Type types = Airspace::Type::all;
      Optional<Airspace::Type> ignored_types;
      bool weather = true;
      Optional<DateTime> datetime;
      Optional<Geometry> geometry;
      Optional<int> buffer = 100;
    };
    using Result   = Outcome<Info, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  /// get_status searches flight advisories for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) = 0;

  /// get_status searches flight advisories for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) = 0;

  /// get_status searches flight advisories for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_status_by_polygon(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) = 0;

 protected:
  Status() = default;
};

}  // namespace airmap

#endif  // AIRMAP_STATUS_H_
