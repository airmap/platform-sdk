//
//  init.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/init.h>

#include <airmap/codec.h>
#include <airmap/paths.h>
#include <airmap/version.h>

#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

#include <cstdlib>

#include <fstream>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;
namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

namespace {
constexpr const char* component{"init"};
constexpr const char* replace_marker{"REPLACE_ME"};
}  // namespace

cmd::Init::Init()
    : cli::CommandWithFlagsAndAction{"init", "initializes user-specific configuration data",
                                     "initializes user-specific configuration data"} {
  flag(flags::config_file(config_file_));
  flag(flags::version(version_));
  flag(cli::make_flag("credentials-type", "type of credentials in {anonymous, oauth}", credentials_type_));
  flag(cli::make_flag("interactive", "enable interactive editing of the final configuration file", interactive_));

  action([this](const cli::Command::Context& ctxt) {
    util::FormattingLogger log{create_default_logger(ctxt.cerr)};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    {
      if (!platform::exists(paths::config_dir(version_)))
        platform::create_directories(paths::config_dir(version_));

      // This scope ensures that the config_file is properly written to disk prior
      // to invoking the user's editor on the file.
      std::ofstream config_file{config_file_.get()};
      if (!config_file) {
        log.errorf(component, "failed to open config file %s for writing", config_file_.get());
        return 1;
      }

      Credentials credentials;
      credentials.api_key = replace_marker;

      switch (credentials_type_) {
        case Credentials::Type::anonymous:
          credentials.anonymous = Credentials::Anonymous{replace_marker};
          break;
        case Credentials::Type::oauth:
          credentials.oauth = Credentials::OAuth{replace_marker, replace_marker, replace_marker, replace_marker};
          break;
      }

      json j = Client::default_configuration(version_, credentials);
      config_file << j.dump(2);

      log.infof(component, "successfully persisted configuration to %s", config_file_.get());
    }

    if (interactive_) {
      std::string editor{"editor"};

      if (auto ve = std::getenv("VISUAL")) {
        editor = ve;
      } else if (auto le = std::getenv("EDITOR")) {
        editor = le;
      }

      // TODO: This does not compile on iOS, need to find a solution here.
      // std::system(fmt::sprintf("%s %s", editor, config_file_.get()).c_str());
    }

    return 0;
  });
}
