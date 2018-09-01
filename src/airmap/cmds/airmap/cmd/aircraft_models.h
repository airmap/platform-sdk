//
//  aircraft_models.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_AIRCRAFT_MODELS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AIRCRAFT_MODELS_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/aircrafts.h>
#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/token.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class AircraftModels : public util::cli::CommandWithFlagsAndAction {
 public:
  AircraftModels();

 private:
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;

  void handle_models_result(const Aircrafts::Models::Result& result, ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<::airmap::Client> client_;
  Required<ConfigFile> config_file_;
  Required<TokenFile> token_file_;
  Optional<Token> token_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AIRCRAFT_MODELS_H_
