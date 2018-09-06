//
//  submit_flight.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/submit_flight.h>

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
constexpr const char* component{"submit-flight"};

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

cmd::SubmitFlight::SubmitFlight()
    : cli::CommandWithFlagsAndAction{"submit-flight", "submits a flight plan for validation/authorization",
                                     "submits a flight plan for validation/authorization"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));
  flag(cli::make_flag("id", "flight plan id", flight_plan_id_));

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

    if (!flight_plan_id_ || !flight_plan_id_.get().validate()) {
      log_.errorf(component, "missing parameter 'id'");
      return 1;
    }

    parameters_.authorization = Token::load_from_json(in_token).id();
    parameters_.id            = flight_plan_id_.get();
    auto result               = ::airmap::Context::create(log_.logger());

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

          auto handler = [this, &ctxt, context, client](const auto& result) {
            if (result) {
              log_.infof(component, "successfully submitted flight plan");
              print_flight_plan(ctxt.cout, result.value());
              context->stop();
            } else {
              log_.errorf(component, "failed to submit flight plan: %s", result.error());
              context->stop(::airmap::Context::ReturnCode::error);
            }
          };

          client->flight_plans().submit(parameters_, handler);
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
