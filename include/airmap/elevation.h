// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef AIRMAP_ELEVATION_H_
#define AIRMAP_ELEVATION_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/error.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>
#include <airmap/visibility.h>

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

/// Elevation provides elevation data for all locations on the surface of the earth at a resolution of 10 meters.
class AIRMAP_EXPORT Elevation : DoNotCopyOrMove {
 public:
  /// Bounds bundles up the longitude/latitude coordinates of the south-west and north-east corners of the rectangle.
  struct AIRMAP_EXPORT Bounds {
    std::vector<float> sw;  ///< Array of latitude/longitude points describing the south-west corner of the rectangle.
    std::vector<float> ne;  ///< Array of latitude/longitude points describing the north-east corner of the rectangle.
  };

  /// Stats bundles up attributes describing elevation statistics.
  struct AIRMAP_EXPORT Stats {
    std::int32_t max;  ///< The maximum elevation in [m].
    std::int32_t min;  ///< The minimum elevation in [m].
    float avg;         ///< The average elevation in [m].
  };

  /// Carpet bundles together a concatenation of elevation values covering the rectangular area defined by the two given
  /// corners and inflated to the nearest outer pixels of the underlying elevation tiles.
  struct AIRMAP_EXPORT Carpet {
    Bounds bounds;                           ///< Bounds of the carpet.
    Stats stats;                             ///< Elevation statistics.
    std::vector<std::vector<float>> carpet;  ///< Array of rows orientated south-west to north-east.
  };

  /// Path bundles up the profile of the path described by the given latitude/longitude milestones.
  struct AIRMAP_EXPORT Path {
    std::vector<float> from;            ///< Array
    std::vector<float> to;              ///< Array
    std::vector<float> step;            ///< Array
    std::vector<std::int32_t> profile;  ///< Array
  };

  /// GetElevationPoints bundles up types to ease interaction
  /// with Elevation::get_elevation*.
  struct AIRMAP_EXPORT GetElevationPoints {
    /// Parameters bundles up input parameters.
    struct Parameters {
      Required<std::string> points;  ///< Array of latitude/longitude points.
    };
    /// Result models the outcome of calling Elevation::get_elevation*.
    using Result = Outcome<std::vector<std::int32_t>, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Elevation::get_elevation* finishes.
    using Callback = std::function<void(const Result&)>;
  };

  /// GetElevationCarpet bundles up types to ease interaction
  /// with Elevation::get_elevation*.
  struct AIRMAP_EXPORT GetElevationCarpet {
    /// Parameters bundles up input parameters.
    struct Parameters {
      Required<std::string>
          points;  ///< Array of latitude/longitude points of the south-west and north-east corners of the rectangle.
    };
    /// Result models the outcome of calling Elevation::get_elevation*.
    using Result = Outcome<Carpet, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Elevation::get_elevation* finishes.
    using Callback = std::function<void(const Result&)>;
  };

  /// GetElevationPath bundles up types to ease interaction
  /// with Elevation::get_elevation*.
  struct AIRMAP_EXPORT GetElevationPath {
    /// Parameters bundles up input parameters.
    struct Parameters {
      Required<std::string> points;  ///< Array of latitude/longitude points along a path.
    };
    /// Result models the outcome of calling Elevation::get_elevation*.
    using Result = Outcome<std::vector<Path>, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Elevation::get_elevation* finishes.
    using Callback = std::function<void(const Result&)>;
  };

  /// get_elevation_points searches elevations for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_elevation_points(const GetElevationPoints::Parameters& parameters,
                                    const GetElevationPoints::Callback& cb) = 0;

  /// get_elevation_carpet searches elevations for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_elevation_carpet(const GetElevationCarpet::Parameters& parameters,
                                    const GetElevationCarpet::Callback& cb) = 0;

  /// get_elevation_path searches elevations for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_elevation_path(const GetElevationPath::Parameters& parameters,
                                  const GetElevationPath::Callback& cb) = 0;

 protected:
  /// @cond
  Elevation() = default;
  /// @endcond
};

}  // namespace airmap

#endif  // AIRMAP_ELEVATION_H_
