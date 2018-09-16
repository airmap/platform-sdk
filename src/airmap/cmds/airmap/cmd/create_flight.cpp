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
//  create_flight.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/create_flight.h>

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

void print_flight(std::ostream& out, const airmap::Flight& flight) {
  cli::TabWriter tw;
  tw << "id"
     << "pilot"
     << "aircraft"
     << "latitude"
     << "longitude"
     << "created-at"
     << "start-time"
     << "end-time" << cli::TabWriter::NewLine{} << flight.id << flight.pilot.id << flight.aircraft.id << flight.latitude
     << flight.longitude << airmap::iso8601::generate(flight.created_at) << airmap::iso8601::generate(flight.start_time)
     << airmap::iso8601::generate(flight.end_time);
  tw.flush(out);
}

constexpr const char* component{"create-flight"};
}  // namespace

cmd::CreateFlight::CreateFlight()
    : cli::CommandWithFlagsAndAction{"create-flight", "creates a flight and registers it with the AirMap services",
                                     "creates a flight and registers it with the AirMap services"} {
  params_.start_time = Clock::universal_time();
  params_.end_time   = params_.start_time + minutes(5);

  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));
  flag(cli::make_flag("latitude", "latitude of take-off point", params_.latitude));
  flag(cli::make_flag("longitude", "longitude of take-off point", params_.longitude));
  flag(cli::make_flag("max-altitude", "maximum altitude reached during flight", params_.max_altitude));
  flag(cli::make_flag("aircraft-id", "id of aircraft that executes the flight", params_.aircraft_id));
  flag(cli::make_flag("start-time", "planned start-time of flight", params_.start_time));
  flag(cli::make_flag("end-time", "planned end-time of flight", params_.end_time));
  flag(cli::make_flag("is-public", "mark the flight as publicly visible on dashboards", params_.is_public));
  flag(cli::make_flag("give-digital-notice", "give digital notice to regulators", params_.give_digital_notice));
  flag(cli::make_flag("buffer", "radius of flight zone centered around the take-off point", params_.buffer));
  flag(cli::make_flag("geometry-file", "use the polygon defined in this geojson file", geometry_file_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger(create_filtering_logger(log_level_, create_default_logger(ctxt.cerr)));

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    if (!token_file_) {
      token_file_ = TokenFile{paths::token_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    auto config = Client::load_configuration_from_json(in_config);

    std::ifstream in_token{token_file_.get()};
    if (!in_token) {
      log_.errorf(component, "failed to open token file %s for reading", token_file_);
      return 1;
    }

    params_.authorization = Token::load_from_json(in_token).id();

    if (geometry_file_) {
      std::ifstream in{geometry_file_.get()};
      if (!in) {
        log_.errorf(component, "failed to open %s for reading", geometry_file_.get());
        return 1;
      }
      Geometry geometry = json::parse(in);
      params_.geometry  = geometry;
    }

    if (!params_.latitude) {
      log_.errorf(component, "missing parameter 'latitude'");
      return 1;
    }

    if (!params_.longitude) {
      log_.errorf(component, "missing parameter 'longitude'");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, &ctxt, config, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Flights::CreateFlight::Result& result) {
            if (result) {
              print_flight(ctxt.cout, result.value());
              context->stop();
            } else {
              log_.errorf(component, "failed to create flight: %s", result.error());
              context->stop(::airmap::Context::ReturnCode::error);
            }
          };

          if (!params_.geometry)
            client->flights().create_flight_by_point(params_, handler);
          else
            client->flights().create_flight_by_polygon(params_, handler);
        });

    return context->exec({SIGINT, SIGQUIT},
                         [this, context](int sig) {
                           log_.infof(component, "received [%s], shutting down", ::strsignal(sig));
                           context->stop();
                         }) == ::airmap::Context::ReturnCode::success
               ? 0
               : 1;
  });
}
