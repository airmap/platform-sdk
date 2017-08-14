#include <airmap/cmds/airmap/cmd/subscribe_traffic.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"traffic"};
}

cmd::SubscribeTraffic::SubscribeTraffic()
    : cli::CommandWithFlagsAndAction{cli::Name{"traffic"}, cli::Usage{"Traffic Alerts and Situational Awareness"},
                                     cli::Description{"receive traffic alerts for a flight with AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"authorization"},
                      cli::Description{"token used for authorizing with the AirMap services"}, params_.authorization));
  flag(cli::make_flag(cli::Name{"flight-id"}, cli::Description{"traffic alerts are sent for this flight id"},
                      params_.flight_id));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_default_logger()};

    if (!params_.api_key) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!params_.api_key.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.authorization) {
      log_.errorf(component, "missing parameter 'authorization'");
      return 1;
    }

    if (!params_.authorization.get().validate()) {
      log_.errorf(component, "parameter 'authorization' for accessing AirMap services must not be empty");
      return 1;
    }

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
      ctxt.cout << "Could not acquire resources for accessing AirMap services" << std::endl;
      return 1;
    }

    auto context = result.value();

    context->create_client_with_credentials(
        Client::Credentials{params_.api_key.get()},
        [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Traffic::Subscribe::Result& result) {
            if (result) {
              ctxt.cout << "TBD: " << result.value().TBD << std::endl;
            } else
              ctxt.cout << "Failed to subscribe TBD " << std::endl;

            context->stop();
          };

          client->traffic().subscribe(Traffic::Subscribe::Params{params_.flight_id.get(), params_.authorization.get()},
                                      handler);
        });

    if (result)
      result.value()->run();

    return 0;
  });
}
