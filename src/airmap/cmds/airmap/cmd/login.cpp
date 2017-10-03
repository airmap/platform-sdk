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
}  // namespace

cmd::Login::Login()
    : cli::CommandWithFlagsAndAction{"login", "logs in to the AirMap services", "logs in to the AirMap services"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::anonymous_token_file(anonymous_token_file_));
  flag(flags::oauth_token_file(oauth_token_file_));
  flag(flags::refreshed_token_file(refreshed_token_file_));
  flag(cli::make_flag("renew", "renew the current token", renew_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cout))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    if (!anonymous_token_file_) {
      anonymous_token_file_ = AnonymousTokenFile{paths::anonymous_token_file(version_).string()};
    }

    if (!oauth_token_file_) {
      oauth_token_file_ = OAuthTokenFile{paths::oauth_token_file(version_).string()};
    }

    if (!refreshed_token_file_) {
      refreshed_token_file_ = RefreshedTokenFile{paths::refreshed_token_file(version_).string()};
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
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
            try {
              std::rethrow_exception(result.error());
            } catch (const std::exception& e) {
              log_.errorf(component, "failed to create client: %s", e.what());
            } catch (...) {
              log_.errorf(component, "failed to create client");
            }
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();
          if (renew_) {
            std::ifstream oauth_token_file{oauth_token_file_.get()};
            if (oauth_token_file_) {
              auto token = Token::load_from_json(oauth_token_file);
              if (token.type() != Token::Type::oauth || token.oauth().refresh.empty()) {
                log_.errorf(component, "oauth token file %s does not hold renewable token", oauth_token_file_.get());
                context_->stop(::airmap::Context::ReturnCode::error);
              } else {
                renew_authentication(config.credentials, token);
              }
            } else {
              log_.errorf(component, "failed to open oauth token file %s for reading", oauth_token_file_.get());
              context_->stop(::airmap::Context::ReturnCode::error);
            }
          } else {
            request_authentication(config.credentials);
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

void cmd::Login::renew_authentication(const Credentials& credentials, const Token& token) {
  Authenticator::RenewAuthentication::Params params;
  params.client_id     = credentials.oauth.get().client_id;
  params.refresh_token = token.oauth().refresh;
  client_->authenticator().renew_authentication(
      params, std::bind(&Login::handle_result_for_renewed_authentication, this, ph::_1));
}

void cmd::Login::request_authentication(const Credentials& credentials) {
  if (credentials.oauth) {
    Authenticator::AuthenticateWithPassword::Params params;
    params.oauth = credentials.oauth.get();
    client_->authenticator().authenticate_with_password(
        params, std::bind(&Login::handle_result_for_authentication_with_password, this, ph::_1));
  } else if (credentials.anonymous) {
    client_->authenticator().authenticate_anonymously(
        {credentials.anonymous.get().id}, std::bind(&Login::handle_result_for_anonymous_authentication, this, ph::_1));
  }
}

void cmd::Login::handle_result_for_authentication_with_password(
    const Authenticator::AuthenticateWithPassword::Result& result) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    auto tfn = oauth_token_file_.get().string();
    if (std::ofstream oauth_token_file{tfn}) {
      nlohmann::json j = Token{result.value()};
      oauth_token_file << j.dump(2);
      log_.infof(component, "successfully persisted login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::success);
    } else {
      log_.errorf(component, "failed to persist login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not authenticate with the Airmap services: %s", e.what());
    } catch (...) {
      log_.errorf(component, "could not authenticate with the Airmap services");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Login::handle_result_for_anonymous_authentication(
    const Authenticator::AuthenticateAnonymously::Result& result) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    auto tfn = anonymous_token_file_.get().string();
    if (std::ofstream anonymous_token_file{tfn}) {
      nlohmann::json j = Token{result.value()};
      anonymous_token_file << j.dump(2);
      log_.infof(component, "successfully persisted login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::success);
    } else {
      log_.errorf(component, "failed to persist login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not authenticate with the Airmap services: %s", e.what());
    } catch (...) {
      log_.errorf(component, "could not authenticate with the Airmap services");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Login::handle_result_for_renewed_authentication(const Authenticator::RenewAuthentication::Result& result) {
  if (result) {
    log_.infof(component, "successfully authenticated with the AirMap services");
    auto tfn = refreshed_token_file_.get().string();
    if (std::ofstream refreshed_token_file{tfn}) {
      nlohmann::json j = Token{result.value()};
      refreshed_token_file << j.dump(2);
      log_.infof(component, "successfully persisted login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::success);
    } else {
      log_.errorf(component, "failed to persist login token to %s", tfn);
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "could not authenticate with the Airmap services: %s", e.what());
    } catch (...) {
      log_.errorf(component, "could not authenticate with the Airmap services");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
