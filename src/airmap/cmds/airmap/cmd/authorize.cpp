#include <airmap/cmds/airmap/cmd/authorize.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

cmd::Authorize::Authorize()
    : cli::CommandWithFlagsAndAction{cli::Name{"authorize"}, cli::Usage{"authorize with the AirMap services"},
                                     cli::Description{"authorize with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"user-id"},
                      cli::Description{"user-id used for paramsymous authorizing with the AirMap services"},
                      params_.anon.user_id));
  flag(cli::make_flag(cli::Name{"client-id"},
                      cli::Description{"client-id used for authorizing with the AirMap services"},
                      params_.passwd.client_id));
  flag(cli::make_flag(cli::Name{"connection-name"},
                      cli::Description{"connection-name used for authorizing with the AirMap services"},
                      params_.passwd.connection_name));
  flag(cli::make_flag(cli::Name{"username"}, cli::Description{"username used for authorizing with the AirMap services"},
                      params_.passwd.username));
  flag(cli::make_flag(cli::Name{"password"}, cli::Description{"password used for authorizing with the AirMap services"},
                      params_.passwd.password));
  flag(cli::make_flag(cli::Name{"device"}, cli::Description{"device used for authorizing with the AirMap services"},
                      params_.passwd.device));

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

          // if user-id parameter found, authenticate anonymously

          if (params_.anon.user_id.size()) {
            auto handler = [this, &ctxt, context,
                            client](const Authenticator::AuthenticateAnonymously::Result& result) {
              if (result)
                ctxt.cout << "Authenticated successfully and received id: " << result.value().id << std::endl;
              else
                ctxt.cout << "Failed to authorize " << params_.anon.user_id << std::endl;

              context->stop();
            };

            client->authenticator().authenticate_anonymously(params_.anon, handler);

          } else {
            auto handler = [this, &ctxt, context,
                            client](const Authenticator::AuthenticateWithPassword::Result& result) {
              if (result) {
                ctxt.cout << "Authenticated successfully and received id: " << result.value().id << std::endl;
              } else
                ctxt.cout << "Failed to authorize " << params_.passwd.username << std::endl;

              context->stop();
            };

            client->authenticator().authenticate_with_password(params_.passwd, handler);
          }

        });

    if (result)
      result.value()->run();

    return 0;
  });
}