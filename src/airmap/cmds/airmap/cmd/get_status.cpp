#include <airmap/cmds/airmap/cmd/get_status.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>

#include <iostream>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

constexpr const char* component{"get-status"};
}  // namespace

cmd::GetStatus::GetStatus()
    : cli::CommandWithFlagsAndAction{cli::Name{"get-status"},
                                     cli::Usage{"checks flight status with the AirMap services"},
                                     cli::Description{"checks flight status with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"version"}, cli::Description{"work against this version of the AirMap services"},
                      version_));
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"latitude"}, cli::Description{"latitude of take-off point"}, params_.latitude));
  flag(cli::make_flag(cli::Name{"longitude"}, cli::Description{"longitude of take-off point"}, params_.longitude));
  flag(cli::make_flag(cli::Name{"weather"}, cli::Description{"report weather conditions"}, params_.weather));
  flag(cli::make_flag(cli::Name{"buffer"}, cli::Description{"radius of flight zone centered around the take-off point"},
                      params_.buffer));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger(create_default_logger(ctxt.cout));

    if (!api_key_) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!api_key_.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto config  = Client::default_configuration(version_, Client::Credentials{api_key_.get()});

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
          if (not result)
            return;

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Status::GetStatus::Result& result) {
            if (result) {
              log_.infof(component, "received status with max_safe_distance: %d and advisory_color: %s\n",
                         result.value().max_safe_distance, Status::get_color_string(result.value().advisory_color));
              // TBD - make proper test and log
              for (const auto a : result.value().advisories) {
                std::cout << "Name: " << a.airspace.name() << std::endl << "Color: " << Status::get_color_string(a.color) << std::endl;
              }
            } else {
              try {
                std::rethrow_exception(result.error());
              } catch (const std::exception& e) {
                log_.errorf(component, "failed to get flight status: %s", e.what());
              } catch (...) {
                log_.errorf(component, "failed to get flight status");
              }
              context->stop();
              return;
            }
            context->stop();
          };

          client->status().get_status_by_point(params_, handler);
        });

    context->run();

    return 0;
  });
}
