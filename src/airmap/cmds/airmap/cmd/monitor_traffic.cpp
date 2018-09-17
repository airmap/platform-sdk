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
#include <airmap/cmds/airmap/cmd/monitor_traffic.h>

#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/paths.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"monitor-traffic"};
}

cmd::MonitorTraffic::MonitorTraffic()
    : cli::CommandWithFlagsAndAction{"monitor-traffic", "monitors traffic alerts and situational awareness",
                                     "monitors traffic alerts and situational awareness"} {
  flag(flags::log_level(params_.log_level));
  flag(flags::config_file(params_.config_file));
  flag(flags::token_file(params_.token_file));
  flag(flags::flight_id(params_.flight_id));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(params_.log_level, create_default_logger(ctxt.cerr))};

    if (!params_.config_file) {
      params_.config_file = ConfigFile{paths::config_file(Client::Version::production).string()};
    }

    if (!params_.token_file) {
      params_.token_file = TokenFile{paths::token_file(Client::Version::production).string()};
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
      log_.errorf(component, "parameter 'flight_id' for accessing AirMap services must not be empty");
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
               "  credentials.api_key: %s",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(
        config, [this, token](const ::airmap::Context::ClientCreateResult& result) {
          if (!result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();

          auto handler = [this](const Traffic::Monitor::Result& result) {
            if (result) {
              log_.infof(component, "successfully created traffic monitor");
              monitor_ = result.value();
              monitor_->subscribe(
                  std::make_shared<::airmap::Traffic::Monitor::LoggingSubscriber>(component, log_.logger()));
            } else {
              log_.errorf(component, "failed to create traffic monitor: %s", result.error());
              context_->stop(::airmap::Context::ReturnCode::error);
              return;
            }
          };

          client_->traffic().monitor(::airmap::Traffic::Monitor::Params{params_.flight_id.get(), token.id()}, handler);
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
