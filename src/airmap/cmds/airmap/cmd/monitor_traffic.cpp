#include <airmap/cmds/airmap/cmd/monitor_traffic.h>

#include <airmap/client.h>
#include <airmap/context.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"monitor-traffic"};
}

cmd::MonitorTraffic::MonitorTraffic()
    : cli::CommandWithFlagsAndAction{"monitor-traffic", "Traffic Alerts and Situational Awareness",
                                     "receive traffic alerts for a flight with AirMap services"} {
  flag(flags::api_key(params_.api_key));
  flag(flags::log_level(params_.log_level));
  flag(flags::authorization(params_.authorization));
  flag(flags::flight_id(params_.flight_id));

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

    context_            = result.value();
    auto credentials    = Credentials{};
    credentials.api_key = params_.api_key.get();
    auto config         = Client::default_configuration(Client::Version::production, credentials);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(config, [this](const ::airmap::Context::ClientCreateResult& result) {
      if (!result) {
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
          context_->stop(::airmap::Context::ReturnCode::error);
          return;
        }
      };

      client_->traffic().monitor(
          ::airmap::Traffic::Monitor::Params{params_.flight_id.get(), params_.authorization.get()}, handler);
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
