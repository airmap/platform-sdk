//
//  aircraft_models.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/aircraft_models.h>

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

constexpr const char* component{"aircraft-models"};

}  // namespace

cmd::AircraftModels::AircraftModels()
    : cli::CommandWithFlagsAndAction{"aircraft-models", "lists all aircraft models in the AirMap database",
                                     "lists all aircraft models in the AirMap database"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));

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

    std::ifstream in_token{token_file_.get()};
    if (!in_token) {
      log_.errorf(component, "failed to open token file %s for reading", token_file_);
      return 1;
    }

    token_ = Token::load_from_json(in_token);

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
            log_.errorf(component, "failed to create AirMap client instance: %s", result.error());
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();
          client_->aircrafts().models(
              Aircrafts::Models::Parameters{},
              std::bind(&AircraftModels::handle_models_result, this, std::placeholders::_1, std::ref(ctxt)));
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

void cmd::AircraftModels::handle_models_result(const Aircrafts::Models::Result& result, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully queried aircraft models");

    cli::TabWriter tw;
    tw << "model-id"
       << "model-name"
       << "manufacturer-id"
       << "manufacturer-name";
    for (const auto& a : result.value()) {
      tw << cli::TabWriter::NewLine{} << a.model.id << a.model.name << a.manufacturer.id << a.manufacturer.name;
    }
    tw.flush(context.get().cout);

    context_->stop();
  } else {
    log_.errorf(component, "failed to query aircraft models");
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}