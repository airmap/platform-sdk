#include <airmap/cmds/airmap/cmd/simulate_telemetry.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/telemetry_simulator.h>

#include <fstream>
#include <iterator>
#include <thread>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

auto polygon =
    airmap::Geometry::polygon({airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}},
                               airmap::Geometry::Coordinate{47.37629618703235, 8.545989990234375,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}},
                               airmap::Geometry::Coordinate{47.37667398429217, 8.548049926757812,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}},
                               airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}}});

constexpr const char* component{"telemetry-simulator"};
}  // namespace

cmd::SimulateTelemetry::SimulateTelemetry()
    : cli::CommandWithFlagsAndAction{"simulate-telemetry", "inject artificial telemetry data for a given flight",
                                     "inject artificial telemetry data for a given flight"} {
  flag(flags::version(params_.version));
  flag(flags::log_level(params_.log_level));
  flag(flags::api_key(params_.api_key));
  flag(flags::authorization(params_.authorization));
  flag(flags::encryption_key(params_.encryption_key));
  flag(flags::telemetry_host(params_.host));
  flag(flags::telemetry_port(params_.port));
  flag(flags::flight_id(params_.flight_id));
  flag(cli::make_flag("frequency", "telemetry is sent with `FREQUENCY` Hz", params_.frequency));
  flag(cli::make_flag("geometry-file", "use the polygon defined in this geojson file", params_.geometry_file));

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

    if (params_.host && !params_.host.get().validate()) {
      log_.errorf(component, "parameter 'host' must not be empty");
      return 1;
    }

    if (!params_.encryption_key) {
      log_.errorf(component, "missing parameter 'encryption-key'");
      return 1;
    }

    if (!params_.encryption_key.get().validate()) {
      log_.errorf(component, "parameter 'encryption-key' must not be empty");
      return 1;
    }

    if (!params_.flight_id) {
      log_.errorf(component, "missing parameter 'flight-id'");
      return 1;
    }

    if (!params_.flight_id.get().validate()) {
      log_.errorf(component, "parameter 'flight-id' must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto config  = Client::default_configuration(params_.version, Client::Credentials{params_.api_key.get()});

    if (params_.host)
      config.telemetry.host = params_.host.get();
    if (params_.port)
      config.telemetry.port = params_.port.get();

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n"
               "  frequency:           %f\n"
               "  flight-id:           %s\n"
               "  enc-key:             %s",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key,
               params_.frequency, params_.flight_id, params_.encryption_key);

    auto geometry = polygon;

    if (params_.geometry_file) {
      std::ifstream in{params_.geometry_file.get()};
      if (!in) {
        log_.errorf(component, "failed to open %s for reading", params_.geometry_file.get());
        return 1;
      }
      geometry = json::parse(in);
    }

    context->create_client_with_configuration(
        config, [this, &ctxt, context, geometry](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          std::thread submitter{[this, &ctxt, geometry, context, client]() {
            Flight flight;
            flight.id = params_.flight_id.get();

            util::TelemetrySimulator simulator{geometry.details_for_polygon()};

            while (true) {
              auto data = simulator.update();

              log_.infof(component, "Submitting update for position (%f,%f)", data.latitude, data.longitude);

              client->telemetry().submit_updates(
                  flight, params_.encryption_key.get(),
                  {Telemetry::Update{Telemetry::Position{milliseconds_since_epoch(Clock::universal_time()),
                                                         data.latitude, data.longitude, 100, 100, 2}}});
              std::this_thread::sleep_for(std::chrono::milliseconds{1000 / params_.frequency});
            }
          }};
          submitter.detach();
        });

    context->run();
    return 0;
  });
}