#include <airmap/cmds/airmap/cmd/authorize_refresh.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"authorize-refresh"};
}

cmd::AuthorizeRefresh::AuthorizeRefresh()
    : cli::CommandWithFlagsAndAction{"authorize-refresh", "renew authorization with the AirMap services",
                                     "renew authorization with the AirMap services"} {
  flag(flags::version(params_.version));
  flag(flags::log_level(params_.log_level));
  flag(flags::api_key(params_.api_key));
  flag(flags::client_id(params_.client_id));
  flag(cli::make_flag("refresh-token", "refresh-token used for authorizing with the AirMap services",
                      params_.refresh_token));

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

    if (!params_.refresh_token) {
      log_.errorf(component, "missing parameter 'refresh-token'");
      return 1;
    }

    if (!params_.refresh_token.get().validate()) {
      log_.errorf(component, "parameter 'refresh-token' for accessing AirMap services must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto config  = Client::default_configuration(params_.version, Client::Credentials{params_.api_key.get()});

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(config, [this, &ctxt,
                                                       context](const ::airmap::Context::ClientCreateResult& result) {
      if (not result)
        return;

      auto client = result.value();

      auto handler = [this, &ctxt, context, client](const Authenticator::RenewAuthentication::Result& result) {
        if (result) {
          log_.infof(component,
                     "successfully refreshed token and received id:\n"
                     "  id:         %s\n"
                     "  expires in: %d",
                     result.value().id, result.value().expires_in.count());
        } else {
          try {
            std::rethrow_exception(result.error());
          } catch (const std::exception& e) {
            log_.errorf(component, "failed to renew %s: %s\n", params_.refresh_token.get(), e.what());
          } catch (...) {
            log_.errorf(component, "failed to renew %s\n", params_.refresh_token.get());
          }
        }

        context->stop();
      };

      client->authenticator().renew_authentication(
          Authenticator::RenewAuthentication::Params{params_.client_id.get(), params_.refresh_token.get()}, handler);

    });

    context->run();

    return 0;
  });
}
