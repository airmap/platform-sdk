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
#include <airmap/cmds/airmap/cmd/plan_flight.h>

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
constexpr const char* component{"plan-flight"};

void print_flight_plan(std::ostream& out, const airmap::FlightPlan& fp) {
  cli::TabWriter tw;

  tw << "id"
     << "pilot-id"
     << "aircraft-id"
     << "takeoff-latitude"
     << "takeoff-longitude"
     << "max-altitude"
     << "min-altitude"
     << "buffer"
     << "start-time"
     << "end-time";
  tw << cli::TabWriter::NewLine{};
  tw << fp.id << fp.pilot.id << fp.aircraft.id << fp.takeoff.latitude << fp.takeoff.longitude << fp.altitude_agl.max
     << fp.altitude_agl.min << fp.buffer << airmap::iso8601::generate(fp.start_time)
     << airmap::iso8601::generate(fp.end_time);

  tw.flush(out);
}

}  // namespace

cmd::PlanFlight::PlanFlight()
    : cli::CommandWithFlagsAndAction{"plan-flight", "creates a flight plan with the AirMap services",
                                     "creates a flight plan and registers it with the AirMap services"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));
  flag(cli::make_flag("plan", "flight plan file", plan_file_));
  flag(cli::make_flag("update", "update the current flight plan", update_));

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

    if (!plan_file_ || !plan_file_.get().validate()) {
      log_.errorf(component, "missing parameter 'plan'");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    context_ = result.value();

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(
        config, [this, &ctxt, config](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();

          std::ifstream in_token{token_file_.get()};
          if (!in_token) {
            log_.errorf(component, "failed to open token file %s for reading", token_file_);
            return;
          }

          std::ifstream plan_in{plan_file_.get()};
          if (!plan_in) {
            log_.errorf(component, "failed to open %s for reading", plan_file_.get());
            return;
          }

          if (update_) {
            FlightPlans::Update::Parameters params;
            params.flight_plan   = json::parse(plan_in);
            params.authorization = Token::load_from_json(in_token).id();
            client_->flight_plans().update(params, std::bind(&PlanFlight::handle_flight_plan_update_result, this,
                                                             std::placeholders::_1, std::ref(ctxt)));
          } else {
            FlightPlans::Create::Parameters params;
            params               = json::parse(plan_in);
            params.authorization = Token::load_from_json(in_token).id();
            params.start_time    = Clock::universal_time();
            params.end_time      = Clock::universal_time() + minutes(5);
            client_->flight_plans().create_by_polygon(params, std::bind(&PlanFlight::handle_flight_plan_create_result,
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

void cmd::PlanFlight::handle_flight_plan_create_result(const FlightPlans::Create::Result& result,
                                                       ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully created flight plan");
    print_flight_plan(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to create flight plan: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::PlanFlight::handle_flight_plan_update_result(const FlightPlans::Update::Result& result,
                                                       ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully updated flight plan");
    print_flight_plan(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to update flight plan: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
