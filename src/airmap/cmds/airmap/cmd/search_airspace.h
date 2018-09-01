//
//  search_airspace.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_SEARCH_AIRSPACE_H_
#define AIRMAP_CMDS_AIRMAP_CMD_SEARCH_AIRSPACE_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/airspace.h>
#include <airmap/airspaces.h>
#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class SearchAirspace : public util::cli::CommandWithFlagsAndAction {
 public:
  SearchAirspace();

 private:
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;
  using GeometryFile    = util::TaggedString<util::tags::MustNotBeEmpty>;
  using AirspaceId      = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Date_Time       = util::TaggedString<util::tags::MustNotBeEmpty>;

  void handle_airspace_for_ids_result(const Airspaces::ForIds::Result& result, ConstContextRef context);
  void handle_airspace_search_result(const Airspaces::Search::Result& result, ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<::airmap::Client> client_;
  Required<ConfigFile> config_file_;
  Optional<GeometryFile> geometry_file_;
  Optional<AirspaceId> airspace_id_;
  Optional<Date_Time> date_time_;
  Optional<bool> full_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_SEARCH_AIRSPACE_H_
