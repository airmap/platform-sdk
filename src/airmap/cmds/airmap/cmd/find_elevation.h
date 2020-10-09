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
#ifndef AIRMAP_CMDS_AIRMAP_CMD_FIND_ELEVATION_H_
#define AIRMAP_CMDS_AIRMAP_CMD_FIND_ELEVATION_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/elevation.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class FindElevation : public util::cli::CommandWithFlagsAndAction {
 public:
  FindElevation();

 private:
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;
  using PointsCSV       = util::TaggedString<util::tags::MustNotBeEmpty>;
  using QueryType       = util::TaggedString<util::tags::MustNotBeEmpty>;

  void handle_elevation_points_result(const Elevation::GetElevationPoints::Result& result, ConstContextRef context);
  void handle_elevation_carpet_result(const Elevation::GetElevationCarpet::Result& result, ConstContextRef context);
  void handle_elevation_path_result(const Elevation::GetElevationPath::Result& result, ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<::airmap::Client> client_;
  Required<ConfigFile> config_file_;
  Required<PointsCSV> points_;
  Required<QueryType> type_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_FIND_ELEVATION_H_
