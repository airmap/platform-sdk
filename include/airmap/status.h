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
  enum class Color { green = 0, yellow = 1, orange = 2, red = 3 };

  struct Advisory {
    Airspace airspace;
    Color color;
  };

  struct Report {
    std::uint64_t max_safe_distance;
    Color advisory_color;
    std::vector<Advisory> advisories;
  };

  struct GetStatus {
    struct Parameters {
      float latitude;
      float longitude;
      Airspace::Type types = Airspace::Type::all;
      Optional<Airspace::Type> ignored_types;
      bool weather = true;
      Optional<DateTime> flight_date_time;
      Optional<Geometry> geometry;
      Optional<int> buffer = 100;
    };
    using Result   = Outcome<Report, std::exception_ptr>;
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

  static const std::string get_color_string(const Color color) {
    switch (color) {
      case Color::green:
        return "green";
      case Color::yellow:
        return "yellow";
      case Color::orange:
        return "orange";
      case Color::red:
        return "red";
      default:
        return "";
    }
  }

 protected:
  Status() = default;
};

}  // namespace airmap

#endif  // AIRMAP_STATUS_H_
