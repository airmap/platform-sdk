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
  cli::TabWriter tw;

  tw << "max-safe-distance"
     << "advisory-color" << cli::TabWriter::NewLine{} << report.max_safe_distance << report.advisory_color;

  if (!report.advisories.empty()) {
    tw << cli::TabWriter::NewLine{} << "name"
       << "color";
    for (const auto& a : report.advisories) {
      tw << cli::TabWriter::NewLine{} << a.airspace.name() << a.color;
    }
  }

  if (report.weather.condition != "") {
    tw << cli::TabWriter::NewLine{} << "condition"
       << "temperature"
       << "wind-speed" << cli::TabWriter::NewLine{} << report.weather.condition << report.weather.temperature
       << report.weather.wind.speed;
  }

  tw.flush(out);
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
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cerr))};

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

    if (!params_.latitude) {
      log_.errorf(component, "missing parameter 'latitude'");
      return 1;
    }

    if (!params_.longitude) {
      log_.errorf(component, "missing parameter 'longitude'");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
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
            log_.errorf(component, "failed to create client: %s", result.error());
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Status::GetStatus::Result& result) {
            if (result) {
              log_.infof(component, "successfully received status");
              print_status(ctxt.cout, result.value());
              context->stop();
            } else {
              log_.errorf(component, "failed to get flight status: %s", result.error());
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
