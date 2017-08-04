#include <airmap/cmds/airmap/cmd/authorize-password.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

cmd::AuthorizePassword::AuthorizePassword()
    : cli::CommandWithFlagsAndAction{cli::Name{"authorize-password"}, cli::Usage{"authorize with the AirMap services"},
                                     cli::Description{"authorize with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"client-id"},
                      cli::Description{"client-id used for authorizing with the AirMap services"}, params_.client_id));
  flag(cli::make_flag(cli::Name{"connection-name"},
                      cli::Description{"connection-name used for authorizing with the AirMap services"},
                      params_.connection_name));
  flag(cli::make_flag(cli::Name{"username"}, cli::Description{"username used for authorizing with the AirMap services"},
                      params_.username));
  flag(cli::make_flag(cli::Name{"password"}, cli::Description{"password used for authorizing with the AirMap services"},
                      params_.password));
  flag(cli::make_flag(cli::Name{"device"}, cli::Description{"device used for authorizing with the AirMap services"},
                      params_.device));

  action([this](const cli::Command::Context& ctxt) {
    auto result = ::airmap::Context::create(create_default_logger());

    if (!result) {
      ctxt.cout << "Could not acquire resources for accessing AirMap services" << std::endl;
      return 1;
    }

    auto context = result.value();

    context->create_client_with_credentials(
        Client::Credentials{api_key_}, [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Authenticator::AuthenticateWithPassword::Result& result) {
            if (result) {
              ctxt.cout << "Authenticated successfully and received id: " << result.value().id << std::endl;
            } else
              ctxt.cout << "Failed to authorize " << params_.username << std::endl;

            context->stop();
          };

          client->authenticator().authenticate_with_password(params_, handler);

        });

    if (result)
      result.value()->run();

    return 0;
  });
}
