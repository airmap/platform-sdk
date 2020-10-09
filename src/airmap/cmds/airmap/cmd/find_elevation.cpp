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
#include <airmap/cmds/airmap/cmd/find_elevation.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/paths.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

constexpr const char* component{"find-elevation"};

void print_elevation_points(std::ostream& out, const std::vector<std::int32_t>& v) {
  cli::TabWriter tw;
  tw << "elevation" << cli::TabWriter::NewLine{};
  for (const auto& e : v)
    tw << e << cli::TabWriter::NewLine{};
  tw.flush(out);
}

void print_elevation_carpet(std::ostream& out, const airmap::Elevation::Carpet c) {
  cli::TabWriter tw;
  tw << "max"
     << "min"
     << "avg";

  tw << cli::TabWriter::NewLine{};
  tw << c.stats.max << c.stats.min << c.stats.avg;

  tw << cli::TabWriter::NewLine{};
  tw.flush(out);
}

void print_elevation_path(std::ostream& out, const std::vector<airmap::Elevation::Path>& v) {
  cli::TabWriter tw;
  tw << "profile" << cli::TabWriter::NewLine{};
  for (const auto& p : v) {
    for (const auto& pr : p.profile) {
      tw << pr << cli::TabWriter::NewLine{};
    }
  }
  tw.flush(out);
}

}  // namespace

cmd::FindElevation::FindElevation()
    : cli::CommandWithFlagsAndAction{"find-elevation", "finds elevation for given coordinates",
                                     "provides elevation data for all locations on the surface of the earth"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(cli::make_flag("points", "comma-separated list of coordinates", points_));
  flag(cli::make_flag("type", "type of elevation query (points, carpet, or path)", type_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cerr))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    if (!points_) {
      log_.errorf(component, "missing parameter 'points'");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    context_    = result.value();
    auto config = Client::load_configuration_from_json(in_config);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(
        config, [this, &ctxt](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();

          if (type_.get().string() == "carpet") {
            Elevation::GetElevationCarpet::Parameters params;
            params.points = points_.get();
            client_->elevation().get_elevation_carpet(params, std::bind(&FindElevation::handle_elevation_carpet_result,
                                                                        this, std::placeholders::_1, std::ref(ctxt)));
          } else if (type_.get().string() == "path") {
            Elevation::GetElevationPath::Parameters params;
            params.points = points_.get();
            client_->elevation().get_elevation_path(params, std::bind(&FindElevation::handle_elevation_path_result,
                                                                      this, std::placeholders::_1, std::ref(ctxt)));
          } else {
            Elevation::GetElevationPoints::Parameters params;
            params.points = points_.get();
            client_->elevation().get_elevation_points(params, std::bind(&FindElevation::handle_elevation_points_result,
                                                                        this, std::placeholders::_1, std::ref(ctxt)));
          }
        });

    return context_->exec({SIGINT, SIGQUIT},
                          [this](int sig) {
                            log_.infof(component, "received [%s], shutting down", ::strsignal(sig));
                            context_->stop();
                          }) == ::airmap::Context::ReturnCode::success
               ? 0
               : 1;
  });
}

void cmd::FindElevation::handle_elevation_points_result(const Elevation::GetElevationPoints::Result& result,
                                                        ConstContextRef context) {
  if (result) {
    log_.infof(component, "succesfully obtained elevation for points\n");
    print_elevation_points(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to obtain elevation: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
    return;
  }
}

void cmd::FindElevation::handle_elevation_carpet_result(const Elevation::GetElevationCarpet::Result& result,
                                                        ConstContextRef context) {
  if (result) {
    log_.infof(component, "succesfully obtained elevation for carpet\n");
    print_elevation_carpet(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to obtain elevation: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
    return;
  }
}
void cmd::FindElevation::handle_elevation_path_result(const Elevation::GetElevationPath::Result& result,
                                                      ConstContextRef context) {
  if (result) {
    log_.infof(component, "succesfully obtained elevation for path\n");
    print_elevation_path(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to obtain elevation: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
    return;
  }
}
