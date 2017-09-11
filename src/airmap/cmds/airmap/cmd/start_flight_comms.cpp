#include <airmap/cmds/airmap/cmd/start_flight_comms.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/util/telemetry_simulator.h>

#include <signal.h>

#include <fstream>
#include <iterator>
#include <thread>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {
constexpr const char* component{"start-flight-comms"};
}

cmd::StartFlightComms::StartFlightComms()
    : cli::CommandWithFlagsAndAction{"start-flight-comms", "prepare flight for injection of telemetry data",
                                     "prepare flight for injection of telemetry data"} {
  flag(flags::version(params_.version));
  flag(flags::log_level(params_.log_level));
  flag(flags::api_key(params_.api_key));
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
      log_.errorf(component, "parameter 'flight-id' for accessing AirMap services must not be empty");
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
               "  credentials.api_key: %s",
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

          client->flights().start_flight_communications(
              Flights::StartFlightCommunications::Parameters{params_.authorization.get(), params_.flight_id.get()},
              [this, &ctxt, context, client](const Flights::StartFlightCommunications::Result& result) {
                if (!result) {
                  try {
                    std::rethrow_exception(result.error());
                  } catch (const std::exception& e) {
                    log_.errorf(component, "failed to start flight communications: %s\n", e.what());
                  } catch (...) {
                    log_.errorf(component, "failed to start flight communications");
                  }

                  context->stop(::airmap::Context::ReturnCode::error);
                  return;
                }

                log_.infof(component, "Started flight communications: %s", result.value().key);
                context->stop();
              });
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
