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
#include <airmap/cmds/airmap/cmd/add_aircraft.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/paths.h>
#include <airmap/rest/client.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

constexpr const char* component{"add-aircraft"};

}  // namespace

cmd::AddAircraft::AddAircraft()
    : cli::CommandWithFlagsAndAction{"add-aircraft", "add an aircraft owned by a pilot",
                                     "add an aircraft owned by a pilot"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));
  flag(cli::make_flag("model-id", "id of the aircraft model", model_id_));
  flag(cli::make_flag("nick-name", "nick-name of the aircraft", nick_name_));

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

    if (!model_id_) {
      log_.errorf(component, "missing parameter 'model-id'");
      return 1;
    }

    if (!model_id_.get().validate()) {
      log_.errorf(component, "parameter 'model-id' must not be empty");
      return 1;
    }

    if (!nick_name_) {
      log_.errorf(component, "missing parameter 'nick-name'");
      return 1;
    }

    if (!nick_name_.get().validate()) {
      log_.errorf(component, "parameter 'nick-name' must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
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
          auto c = dynamic_cast<::airmap::rest::Client*>(client_.get());
          if (c && token_) {
            c->handle_auth_update(token_.get().id());
          }
          
          Pilots::Authenticated::Parameters params;
          params.retrieve_statistics = false;
          client_->pilots().authenticated(params, std::bind(&AddAircraft::handle_authenticated_pilot_result, this,
                                                            std::placeholders::_1, std::ref(ctxt)));
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

void cmd::AddAircraft::handle_authenticated_pilot_result(const Pilots::Authenticated::Result& result,
                                                         ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully queried information about pilot");
    Pilots::AddAircraft::Parameters params;
    params.id            = result.value().id;
    params.model_id      = model_id_.get();
    params.nick_name     = nick_name_.get();

    client_->pilots().add_aircraft(
        params, std::bind(&AddAircraft::handle_add_aircraft_result, this, std::placeholders::_1, context));
  } else {
    log_.errorf(component, "failed to query information about pilot: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::AddAircraft::handle_add_aircraft_result(const Pilots::AddAircraft::Result& result, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully added aircraft to pilot profile");

    cli::TabWriter tw;
    tw << "id"
       << "nick-name"
       << "model-id"
       << "created-at" << cli::TabWriter::NewLine{} << result.value().id << result.value().nick_name
       << result.value().model.model.id << iso8601::generate(result.value().created_at);
    tw.flush(context.get().cout);

    context_->stop();
  } else {
    log_.errorf(component, "failed to add aircraft: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
