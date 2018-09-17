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
#include <airmap/cmds/airmap/cmd/login.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/paths.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;
namespace ph  = std::placeholders;
using json    = nlohmann::json;

namespace {
constexpr const char* component{"login"};

void print_token(std::ostream& out, const airmap::Token& token) {
  cli::TabWriter tw;
  tw << "type"
     << "id"
     << "access"
     << "refresh"
     << "expires-in-seconds" << cli::TabWriter::NewLine{};

  switch (token.type()) {
    case airmap::Token::Type::anonymous:
      tw << token.type() << token.anonymous().id << "n/a"
         << "n/a"
         << "n/a";
      break;
    case airmap::Token::Type::oauth:
      tw << token.type() << token.oauth().id << token.oauth().access << token.oauth().refresh << "n/a";
      break;
    case airmap::Token::Type::refreshed:
      tw << token.type() << token.refreshed().id << "n/a"
         << "n/a" << token.refreshed().expires_in.count();
      break;
    case airmap::Token::Type::unknown:
      tw << "n/a"
         << "n/a"
         << "n/a"
         << "n/a"
         << "n/a";
      break;
  }

  tw.flush(out);
}

}  // namespace

cmd::Login::Login()
    : cli::CommandWithFlagsAndAction{"login", "logs in to the AirMap services", "logs in to the AirMap services"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));
  flag(cli::make_flag("renew", "renew the current token", renew_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cerr))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    if (!token_file_) {
      token_file_ = TokenFile{paths::token_file(version_).string()};
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    context_ = result.value();

    std::ifstream config_file{config_file_.get()};
    if (!config_file) {
      log_.errorf(component, "failed to open config file %s for reading", config_file_.get());
      return 1;
    }
    auto config = Client::load_configuration_from_json(config_file);

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
          if (renew_) {
            std::ifstream token_file{token_file_.get()};
            if (token_file_) {
              auto token = Token::load_from_json(token_file);
              if (token.type() == Token::Type::oauth) {
                if (token.oauth().refresh.empty()) {
                  log_.errorf(component, "token file %s does not hold renewable token", token_file_.get());
                  context_->stop(::airmap::Context::ReturnCode::error);
                } else {
                  renew_authentication(config.credentials, token, std::ref(ctxt));
                }
              } else if (token.type() == Token::Type::refreshed) {
                if (!token.refreshed().original_token) {
                  log_.errorf(component, "token file %s does not hold renewable token", token_file_.get());
                  context_->stop(::airmap::Context::ReturnCode::error);
                } else {
                  renew_authentication(config.credentials, token, std::ref(ctxt));
                }
              } else {
                log_.errorf(component, "token file %s does not hold renewable token", token_file_.get());
                context_->stop(::airmap::Context::ReturnCode::error);
              }
            } else {
              log_.errorf(component, "failed to open token file %s for reading", token_file_.get());
              context_->stop(::airmap::Context::ReturnCode::error);
            }
          } else {
            request_authentication(config.credentials, std::ref(ctxt));
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

void cmd::Login::renew_authentication(const Credentials& credentials, const Token& token, ConstContextRef context) {
  Authenticator::RenewAuthentication::Params params;
  params.client_id = credentials.oauth.get().client_id;

  if (token.type() == Token::Type::oauth) {
    params.refresh_token = token.oauth().refresh;
  } else if (token.type() == Token::Type::refreshed) {
    params.refresh_token = token.refreshed().original_token.get().refresh;
  }

  client_->authenticator().renew_authentication(
      params, std::bind(&Login::handle_result_for_renewed_authentication, this, ph::_1, token, context));
}

void cmd::Login::request_authentication(const Credentials& credentials, ConstContextRef context) {
  if (credentials.oauth) {
    Authenticator::AuthenticateWithPassword::Params params;
    params.oauth = credentials.oauth.get();
    client_->authenticator().authenticate_with_password(
        params, std::bind(&Login::handle_result_for_authentication_with_password, this, ph::_1, context));
  } else if (credentials.anonymous) {
    client_->authenticator().authenticate_anonymously(
        {credentials.anonymous.get().id},
        std::bind(&Login::handle_result_for_anonymous_authentication, this, ph::_1, context));
  }
}

void cmd::Login::handle_result_for_authentication_with_password(
    const Authenticator::AuthenticateWithPassword::Result& result, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    auto token = Token{result.value()};
    print_token(context.get().cout, token);
    auto tfn = token_file_.get().string();
    if (std::ofstream token_file{tfn}) {
      nlohmann::json j = token;
      token_file << j.dump(2);
      log_.infof(component, "successfully persisted login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::success);
    } else {
      log_.errorf(component, "failed to persist login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    log_.errorf(component, "failed to authenticate with the Airmap services: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Login::handle_result_for_anonymous_authentication(
    const Authenticator::AuthenticateAnonymously::Result& result, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    auto token = Token{result.value()};
    print_token(context.get().cout, token);
    auto tfn = token_file_.get().string();
    if (std::ofstream token_file{tfn}) {
      nlohmann::json j = token;
      token_file << j.dump(2);
      log_.infof(component, "successfully persisted login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::success);
    } else {
      log_.errorf(component, "failed to persist login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    log_.errorf(component, "failed to authenticate with the Airmap services: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Login::handle_result_for_renewed_authentication(const Authenticator::RenewAuthentication::Result& result,
                                                          const Token& previous_token, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    auto t = result.value();

    if (previous_token.type() == Token::Type::oauth) {
      t.original_token = previous_token.oauth();
    } else if (previous_token.type() == Token::Type::refreshed) {
      t.original_token = previous_token.refreshed().original_token.get();
    }

    auto token = Token{t};
    print_token(context.get().cout, token);

    auto tfn = token_file_.get().string();
    if (std::ofstream token_file{tfn}) {
      nlohmann::json j = token;
      token_file << j.dump(2);
      log_.infof(component, "successfully persisted login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::success);
    } else {
      log_.errorf(component, "failed to persist login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    log_.errorf(component, "failed to authenticate with the Airmap services: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
