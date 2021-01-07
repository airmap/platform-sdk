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
#include <airmap/cmds/airmap/cmd/pilot.h>

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

constexpr const char* component{"pilot"};

void print_pilot_and_aircrafts(std::ostream& out, const airmap::Pilot& pilot,
                               const std::vector<airmap::Pilot::Aircraft>& aircrafts) {
  cli::TabWriter tw;
  tw << "id"
     << "first-name"
     << "last-name"
     << "email"
     << "username"
     << "picture-url"
     << "email-verified"
     << "phone-verified"
     << "flights-count"
     << "last-flight"
     << "aircrafts-count"
     << "created-at";
  tw << cli::TabWriter::NewLine{};
  tw << pilot.id << pilot.first_name << pilot.last_name << pilot.email << pilot.user_name << pilot.picture_url
     << pilot.verification_status.email << pilot.verification_status.phone << pilot.statistics.flight.total
     << airmap::iso8601::generate(pilot.statistics.flight.last_flight_time) << pilot.statistics.aircraft.total
     << airmap::iso8601::generate(pilot.created_at);

  if (!aircrafts.empty()) {
    tw << cli::TabWriter::NewLine{};
    tw << "id"
       << "nick-name"
       << "model-id"
       << "model-name"
       << "manufacturer-id"
       << "manufacturer-name";
    for (const auto& aircraft : aircrafts) {
      tw << cli::TabWriter::NewLine{} << aircraft.id << aircraft.nick_name << aircraft.model.model.id
         << aircraft.model.model.name << aircraft.model.manufacturer.id << aircraft.model.manufacturer.name;
    }
  }

  tw.flush(out);
}

}  // namespace

cmd::Pilot::Pilot()
    : cli::CommandWithFlagsAndAction{"pilot", "queries information about a pilot",
                                     "queries information about a pilot"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));
  flag(cli::make_flag("pilot-id", "id of pilot", pilot_id_));

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

    if (pilot_id_ && !pilot_id_.get().validate()) {
      log_.errorf(component, "parameter 'pilot-id' must not be empty");
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
            log_.errorf(component, "failed to create AirMap client instance: %s", result.error());
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();

          if (pilot_id_) {
            Pilots::ForId::Parameters params;
            params.authorization       = token_.get().id();
            params.retrieve_statistics = true;
            client_->pilots().for_id(
                params, std::bind(&Pilot::handle_for_id_pilot_result, this, std::placeholders::_1, std::ref(ctxt)));
          } else {
            Pilots::Authenticated::Parameters params;
            params.authorization       = token_.get().id();
            params.retrieve_statistics = true;
            client_->pilots().authenticated(params, std::bind(&Pilot::handle_authenticated_pilot_result, this,
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

void cmd::Pilot::handle_authenticated_pilot_result(const Pilots::Authenticated::Result& result,
                                                   ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile for authenticated user");
    Pilots::Aircrafts::Parameters params;
    params.id            = result.value().id;
    params.authorization = token_.get().id();

    client_->pilots().aircrafts(
        params, std::bind(&Pilot::handle_aircrafts_result, this, result.value(), std::placeholders::_1, context));
  } else {
    log_.errorf(component, "failed to query information about pilot: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Pilot::handle_for_id_pilot_result(const Pilots::ForId::Result& result, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile for id");
    Pilots::Aircrafts::Parameters params;
    params.id            = result.value().id;
    params.authorization = token_.get().id();

    client_->pilots().aircrafts(
        params, std::bind(&Pilot::handle_aircrafts_result, this, result.value(), std::placeholders::_1, context));
  } else {
    log_.errorf(component, "failed to query information about pilot: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Pilot::handle_aircrafts_result(const ::airmap::Pilot& pilot, const Pilots::Aircrafts::Result& result,
                                         ConstContextRef context) {
  if (result) {
    print_pilot_and_aircrafts(context.get().cout, pilot, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to query information about aircrafts: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
