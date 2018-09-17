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
#ifndef AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_
#define AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class SimulateTelemetry : public util::cli::CommandWithFlagsAndAction {
 public:
  SimulateTelemetry();

 private:
  using GeometryFile = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  struct {
    Client::Version version{Client::Version::production};
    Logger::Severity log_level{Logger::Severity::info};
    Required<ConfigFile> config_file;
    Required<TelemetryHost> host;
    Required<std::uint16_t> port;
    std::uint16_t frequency{5};
    Required<FlightId> flight_id;
    Required<EncryptionKey> encryption_key;
    Optional<GeometryFile> geometry_file;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_
