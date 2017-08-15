#include <airmap/cmds/airmap/cmd/subscribe_traffic.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"traffic"};
}

cmd::SubscribeTraffic::SubscribeTraffic()
    : cli::CommandWithFlagsAndAction{cli::Name{"monitor-traffic"},
                                     cli::Usage{"Traffic Alerts and Situational Awareness"},
                                     cli::Description{"receive traffic alerts for a flight with AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"authorization"},
                      cli::Description{"token used for authorizing with the AirMap services"}, params_.authorization));
  flag(cli::make_flag(cli::Name{"flight-id"}, cli::Description{"traffic alerts are sent for this flight id"},
                      params_.flight_id));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_default_logger(ctxt.cout)};

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
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    context_ = result.value();
    auto config =
        Client::default_configuration(Client::Version::production, Client::Credentials{params_.api_key.get()});

    context_->create_client_with_configuration(config, [this](const ::airmap::Context::ClientCreateResult& result) {
      if (!result) {
        try {
          std::rethrow_exception(result.error());
        } catch (const std::exception& e) {
          log_.errorf(component, "failed to create client: %s", e.what());
        } catch (...) {
          log_.errorf(component, "failed to create client");
        }
        context_->stop();
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
          try {
            std::rethrow_exception(result.error());
          } catch (const std::exception& e) {
            log_.errorf(component, "failed to create traffic monitor: %s", e.what());
          } catch (...) {
            log_.errorf(component, "failed to create traffic monitor");
          }
          context_->stop();
          return;
        }
      };

      client_->traffic().monitor(
          ::airmap::Traffic::Monitor::Params{params_.flight_id.get(), params_.authorization.get()}, handler);
    });

    context_->run();
    return 0;
  });
}
