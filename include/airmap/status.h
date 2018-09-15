/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  status.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_STATUS_H_
#define AIRMAP_STATUS_H_

#include <airmap/airspace.h>
#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/error.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>
#include <airmap/visibility.h>

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

/// Status provides functionality to query airspace and weather information about
/// a geographic area.
class AIRMAP_EXPORT Status : DoNotCopyOrMove {
 public:
  /// Color enumerates known colors assigned to advisories.
  enum class Color { green = 0, yellow = 1, orange = 2, red = 3 };

  /// Advisory bundles together airspace information and its evaluation in terms
  /// good to fly/needs information or feedback/conflict.
  struct AIRMAP_EXPORT Advisory {
    Airspace airspace;  /// The airspace that the advisory refers to.
    Color color;        /// The evaluation of the airspace.
  };

  /// Wind bundles up attributes describing a wind conditions.
  struct AIRMAP_EXPORT Wind {
    std::uint32_t heading = 0;  ///< The heading in [°].
    std::uint32_t speed   = 0;  ///< The speed in [°].
    std::uint32_t gusting = 0;
  };

  /// Weather bundles up attributes describing a weather condition.
  struct AIRMAP_EXPORT Weather {
    std::string condition;            ///< The overall weather condition.
    std::string icon;                 ///< The icon or class of icon that should be used for display purposes.
    Wind wind;                        ///< The details about the current wind conditions.
    std::int32_t temperature    = 0;  ///< The temperature in [°C].
    float humidity              = 0.0;
    std::uint32_t visibility    = 0;  ///< Visibility in [m].
    std::uint32_t precipitation = 0;  ///< The probability of precipitation in [%].
  };

  /// Report summarizes information about a geographic area.
  struct AIRMAP_EXPORT Report {
    std::uint32_t max_safe_distance = 0;  ///< The distance to the area that is considered safe in [m].
    Color advisory_color;                 ///< The overall evaluation of all advisories.
    std::vector<Advisory> advisories;     ///< All relevant advisories.
    Weather weather;                      ///< The weather conditions.
  };

  /// GetStatus bundles up types to ease interaction
  /// with Status::get_status*.
  struct AIRMAP_EXPORT GetStatus {
    /// Parameters bundles up input parameters.
    struct AIRMAP_EXPORT Parameters {
      Required<float> latitude;                ///< The latitude of the center point of the query.
      Required<float> longitude;               ///< The longitude of the center point of the query.
      Optional<Airspace::Type> types;          ///< Query status information for these types of airspaces.
      Optional<Airspace::Type> ignored_types;  ///< Ignore these types of airspaces when querying status information.
      Optional<bool> weather;                  ///< If true, weather conditions are included with the status report.
      Optional<DateTime> flight_date_time;     ///< Time when a flight is going to happen.
      Optional<Geometry> geometry;             ///< The geometry for the query.
      Optional<std::uint32_t> buffer;          ///< Buffer around the center point of the query.
    };
    /// Result models the outcome of calling Status::get_status*.
    using Result = Outcome<Report, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Status::get_status* finishes.
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
  /// @cond
  Status() = default;
  /// @endcond
};

/// @cond
AIRMAP_EXPORT std::ostream& operator<<(std::ostream& out, Status::Color color);
AIRMAP_EXPORT std::istream& operator>>(std::istream& in, Status::Color& color);
/// @endcond

}  // namespace airmap

#endif  // AIRMAP_STATUS_H_
