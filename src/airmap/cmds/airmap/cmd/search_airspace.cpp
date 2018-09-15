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
//  search_airspace.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/search_airspace.h>

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

constexpr const char* component{"search-airspace"};

void print_airspace(std::ostream& out, const airmap::Airspace& a) {
  cli::TabWriter tw;

  tw << "id"
     << "name"
     << "type" << cli::TabWriter::NewLine{} << a.id() << a.name() << a.type() << cli::TabWriter::NewLine{};

  tw.flush(out);
}

void print_airspaces(std::ostream& out, const std::vector<airmap::Airspace>& v) {
  cli::TabWriter tw;

  tw << "id"
     << "name"
     << "type";

  for (const auto& a : v) {
    tw << cli::TabWriter::NewLine{};
    tw << a.id() << a.name() << a.type();
  }

  tw << cli::TabWriter::NewLine{};
  tw.flush(out);
}

}  // namespace

cmd::SearchAirspace::SearchAirspace()
    : cli::CommandWithFlagsAndAction{"search-airspace",
                                     "searches for airspace objects by geometry or id from the AirMap services",
                                     "searches for airspace objects by geometry or id from the AirMap services"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(cli::make_flag("geometry-file", "use the polygon defined in this geojson file", geometry_file_));
  flag(cli::make_flag("id", "id of airspace", airspace_id_));
  flag(cli::make_flag("full", "return full airspace object", full_));
  flag(cli::make_flag("date_time", "search for airspace that was active at this time", date_time_));

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

    if (!geometry_file_ && !airspace_id_) {
      log_.errorf(component, "missing parameter 'geometry-file' or 'id'");
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

          if (airspace_id_) {
            Airspaces::ForIds::Parameters params;
            params.id = airspace_id_.get();
            client_->airspaces().for_ids(params, std::bind(&SearchAirspace::handle_airspace_for_ids_result, this,
                                                           std::placeholders::_1, std::ref(ctxt)));
          } else if (geometry_file_) {
            std::ifstream in{geometry_file_.get()};
            if (!in) {
              log_.errorf(component, "failed to open %s for reading", geometry_file_.get());
              return;
            }
            Geometry geometry = json::parse(in);
            Airspaces::Search::Parameters params;
            params.geometry = geometry;
            if (date_time_) {
              params.date_time = iso8601::parse(date_time_.get());
            } else {
              params.date_time = DateTime(Clock::universal_time().date());
            }
            if (full_) {
              params.full = full_.get();
            }
            client_->airspaces().search(params, std::bind(&SearchAirspace::handle_airspace_search_result, this,
                                                          std::placeholders::_1, std::ref(ctxt)));
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

void cmd::SearchAirspace::handle_airspace_search_result(const Airspaces::Search::Result& result,
                                                        ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully obtained airspaces");
    print_airspaces(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to obtain airspaces: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
    return;
  }
}

void cmd::SearchAirspace::handle_airspace_for_ids_result(const Airspaces::ForIds::Result& result,
                                                         ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully obtained airspaces");
    print_airspace(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to obtain airspaces: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
    return;
  }
}
