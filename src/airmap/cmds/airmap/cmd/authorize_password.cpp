#include <airmap/cmds/airmap/cmd/authorize_password.h>

#include <airmap/client.h>
#include <airmap/context.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"authorize-password"};
}

cmd::AuthorizePassword::AuthorizePassword()
    : cli::CommandWithFlagsAndAction{"authorize-password", "authorize with the AirMap services",
                                     "authorize with the AirMap services"} {
  flag(flags::version(params_.version));
  flag(flags::log_level(params_.log_level));
  flag(flags::api_key(params_.api_key));
  flag(flags::client_id(params_.client_id));

  flag(cli::make_flag("username", "username used for authorizing with the AirMap services", params_.username));
  flag(cli::make_flag("password", "password used for authorizing with the AirMap services", params_.password));
  flag(cli::make_flag("device", "device used for authorizing with the AirMap services", params_.device));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(params_.log_level, create_default_logger(ctxt.cout))};

    if (!params_.api_key) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!params_.api_key.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.client_id) {
      log_.errorf(component, "missing parameter 'client-id'");
      return 1;
    }

    if (!params_.client_id.get().validate()) {
      log_.errorf(component, "parameter 'client-id' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.username) {
      log_.errorf(component, "missing parameter 'username'");
      return 1;
    }

    if (!params_.username.get().validate()) {
      log_.errorf(component, "parameter 'username' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.password) {
      log_.errorf(component, "missing parameter 'password'");
      return 1;
    }

    if (!params_.password.get().validate()) {
      log_.errorf(component, "parameter 'password' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.device) {
      log_.errorf(component, "missing parameter 'device'");
      return 1;
    }

    if (!params_.device.get().validate()) {
      log_.errorf(component, "parameter 'device' for accessing AirMap services must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    auto context        = result.value();
    auto credentials    = Credentials{};
    credentials.api_key = params_.api_key.get();
    auto config         = Client::default_configuration(params_.version, credentials);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            try {
              std::rethrow_exception(result.error());
            } catch (const std::exception& e) {
              log_.errorf(component, "failed to create client: %s", e.what());
            } catch (...) {
              log_.errorf(component, "failed to create client");
            }
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Authenticator::AuthenticateWithPassword::Result& result) {
            if (result) {
              log_.infof(component, "successfully authenticated and received id: %s\n", result.value().id);
              context->stop();
            } else {
              try {
                std::rethrow_exception(result.error());
              } catch (const std::exception& e) {
                log_.errorf(component, "failed to authorize %s: %s\n", params_.username.get(), e.what());
              } catch (...) {
                log_.errorf(component, "failed to authorize %s\n", params_.username.get());
              }
              context->stop(::airmap::Context::ReturnCode::error);
            }
          };

          client->authenticator().authenticate_with_password(
              Authenticator::AuthenticateWithPassword::Params{params_.client_id.get(), params_.username.get(),
                                                              params_.password.get(), params_.device.get()},
              handler);
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
