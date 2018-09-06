//
//  start_flight_comms.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/start_flight_comms.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/paths.h>
#include <airmap/util/telemetry_simulator.h>

#include <signal.h>

#include <fstream>
#include <iterator>
#include <thread>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {
constexpr const char* component{"start-flight-comms"};
}

cmd::StartFlightComms::StartFlightComms()
    : cli::CommandWithFlagsAndAction{"start-flight-comms", "prepares flight for injection of telemetry data",
                                     "prepares flight for injection of telemetry data"} {
  flag(flags::version(params_.version));
  flag(flags::log_level(params_.log_level));
  flag(flags::config_file(params_.config_file));
  flag(flags::token_file(params_.token_file));
  flag(flags::flight_id(params_.flight_id));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(params_.log_level, create_default_logger(ctxt.cerr))};

    if (!params_.config_file) {
      params_.config_file = ConfigFile{paths::config_file(params_.version).string()};
    }

    if (!params_.token_file) {
      params_.token_file = TokenFile{paths::token_file(params_.version).string()};
    }

    std::ifstream in_config{params_.config_file.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", params_.config_file);
      return 1;
    }

    std::ifstream in_token{params_.token_file.get()};
    if (!in_token) {
      log_.errorf(component, "failed to open token file %s for reading", params_.token_file);
      return 1;
    }

    auto token = Token::load_from_json(in_token);

    if (!params_.flight_id) {
      log_.errorf(component, "missing parameter 'flight-id'");
      return 1;
    }

    if (!params_.flight_id.get().validate()) {
      log_.errorf(component, "parameter 'flight-id' for accessing AirMap services must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto config  = Client::load_configuration_from_json(in_config);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, &ctxt, token, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          auto client = result.value();

          client->flights().start_flight_communications(
              Flights::StartFlightCommunications::Parameters{token.id(), params_.flight_id.get()},
              [this, &ctxt, context, client](const Flights::StartFlightCommunications::Result& result) {
                if (!result) {
                  log_.errorf(component, "failed to start flight communications: %s", result.error());
                  context->stop(::airmap::Context::ReturnCode::error);
                  return;
                }

                log_.infof(component, "successfully started flight communications");

                cli::TabWriter tw;

                tw << "encryption-key" << cli::TabWriter::NewLine{} << result.value().key;

                tw.flush(ctxt.cout);

                context->stop();
              });
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
