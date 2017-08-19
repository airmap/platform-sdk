#include <airmap/cmds/airmap/cmd/authorize_anonymous.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"authorize-anonymously"};
}

cmd::AuthorizeAnonymous::AuthorizeAnonymous()
    : cli::CommandWithFlagsAndAction{"authorize-anonymously", "anonymously authorize with the AirMap services",
                                     "anonymously authorize with the AirMap services"} {
  flag(flags::version(params_.version));
  flag(flags::log_level(params_.log_level));
  flag(flags::api_key(params_.api_key));
  flag(flags::user_id(params_.user_id));

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

    if (!params_.user_id) {
      log_.errorf(component, "missing parameter 'user-id'");
      return 1;
    }

    if (!params_.user_id.get().validate()) {
      log_.errorf(component, "parameter 'user-id' for accessing AirMap services must not be empty");
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
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Authenticator::AuthenticateAnonymously::Result& result) {
            if (result) {
              log_.infof(component, "Authenticated successfully and received id: %s\n", result.value().id);
            } else {
              try {
                std::rethrow_exception(result.error());
              } catch (const std::exception& e) {
                log_.errorf(component, "failed to authorize %s: %s\n", params_.user_id.get(), e.what());
              } catch (...) {
                log_.errorf(component, "failed to authorize %s\n", params_.user_id.get());
              }
            }

            context->stop();
          };

          client->authenticator().authenticate_anonymously(
              Authenticator::AuthenticateAnonymously::Params{params_.user_id.get()}, handler);
        });

    context->run();

    return 0;
  });
}
