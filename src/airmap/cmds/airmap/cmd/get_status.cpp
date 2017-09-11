#include <airmap/cmds/airmap/cmd/get_status.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/paths.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

void print_status(std::ostream& out, const airmap::Status::Report& report) {
  out << "Received status:     " << std::endl
      << "  max-safe-distance: " << report.max_safe_distance << std::endl
      << "  advisory-color:    " << report.advisory_color << std::endl
      << "  Advisories:        " << std::endl;
  for (const auto& a : report.advisories) {
    out << "    Name:            " << a.airspace.name() << std::endl << "    Color:           " << a.color << std::endl;
  }
  if (report.weather.condition != "") {
    out << "  Weather:           " << std::endl
        << "    condition:       " << report.weather.condition << std::endl
        << "    temperature:     " << report.weather.temperature << std::endl
        << "    wind-speed:      " << report.weather.wind.speed << std::endl;
  }
}

constexpr const char* component{"get-status"};
}  // namespace

cmd::GetStatus::GetStatus()
    : cli::CommandWithFlagsAndAction{"get-status", "checks flight status with the AirMap services",
                                     "checks flight status with the AirMap services"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(cli::make_flag("latitude", "latitude of take-off point", params_.latitude));
  flag(cli::make_flag("longitude", "longitude of take-off point", params_.longitude));
  flag(cli::make_flag("weather", "report weather conditions", params_.weather));
  flag(cli::make_flag("buffer", "radius of flight zone centered around the take-off point", params_.buffer));
  flag(cli::make_flag("flight-date-time", "date and time for planned flight", params_.flight_date_time));
  flag(cli::make_flag("geometry-file", "use the polygon defined in this geojson file", geometry_file_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cout))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    if (geometry_file_) {
      std::ifstream in{geometry_file_.get()};
      if (!in) {
        log_.errorf(component, "failed to open %s for reading", geometry_file_.get());
        return 1;
      }
      Geometry geometry = json::parse(in);
      params_.geometry  = geometry;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto config  = Client::load_configuration_from_json(in_config);

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

          auto handler = [this, &ctxt, context, client](const Status::GetStatus::Result& result) {
            if (result) {
              log_.infof(component, "successfully received status with max-safe-distance: %d and advisory-color: %s\n",
                         result.value().max_safe_distance, result.value().advisory_color);
              print_status(ctxt.cout, result.value());
              context->stop();
            } else {
              try {
                std::rethrow_exception(result.error());
              } catch (const std::exception& e) {
                log_.errorf(component, "failed to get flight status: %s", e.what());
              } catch (...) {
                log_.errorf(component, "failed to get flight status");
              }
              context->stop(::airmap::Context::ReturnCode::error);
              return;
            }
          };

          if (!params_.geometry)
            client->status().get_status_by_point(params_, handler);
          else
            client->status().get_status_by_polygon(params_, handler);
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
