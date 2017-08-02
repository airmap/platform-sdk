#include <airmap/cmds/airmap/cmd/authorize-refresh.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

cmd::AuthorizeRefresh::AuthorizeRefresh()
    : cli::CommandWithFlagsAndAction{cli::Name{"authorize-refresh"},
                                     cli::Usage{"renew authorization with the AirMap services"},
                                     cli::Description{"renew authorization with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"client-id"},
                      cli::Description{"client-id used for authorizing with the AirMap services"}, params_.client_id));
  flag(cli::make_flag(cli::Name{"device"}, cli::Description{"device used for authorizing with the AirMap services"},
                      params_.device));
  flag(cli::make_flag(cli::Name{"id-token"}, cli::Description{"id-token used for authorizing with the AirMap services"},
                      params_.id_token));

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

          auto handler = [this, &ctxt, context, client](const Authenticator::RenewAuthentication::Result& result) {
            if (result) {
              ctxt.cout << "Refreshed token successfully and received id: " << result.value().id << std::endl;
            } else
              ctxt.cout << "Failed to renew " << params_.id_token << std::endl;

            context->stop();
          };

          client->authenticator().renew_authentication(params_, handler);

        });

    if (result)
      result.value()->run();

    return 0;
  });
}