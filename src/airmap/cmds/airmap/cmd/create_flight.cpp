#include <airmap/cmds/airmap/cmd/create_flight.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

void print_flight(std::ostream& out, const airmap::Flight& flight) {
  out << "Created flight with: " << std::endl
      << "  id:         " << flight.id << std::endl
      << "  pilot:      " << flight.pilot.id << std::endl
      << "  aircraft:   " << flight.aircraft.id << std::endl
      << "  latitude:   " << flight.latitude << std::endl
      << "  longitude:  " << flight.longitude << std::endl
      << "  created-at: " << airmap::iso8601::generate(flight.created_at) << std::endl
      << "  start-time: " << airmap::iso8601::generate(flight.start_time) << std::endl
      << "  end-time:   " << airmap::iso8601::generate(flight.end_time) << std::endl;
}

}  // namespace

cmd::CreateFlight::CreateFlight()
    : cli::CommandWithFlagsAndAction{cli::Name{"create-flight"},
                                     cli::Usage{"creates a flight and registers it with the airmap services"},
                                     cli::Description{"creates a flight and registers it with the airmap services"}} {
  params_.start_time = Clock::universal_time();
  params_.end_time   = params_.start_time + Minutes(5);

  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"authorization"},
                      cli::Description{"token used for authorizing with the AirMap services"}, params_.authorization));
  flag(cli::make_flag(cli::Name{"latitude"}, cli::Description{"latitude of take-off point"}, params_.latitude));
  flag(cli::make_flag(cli::Name{"longitude"}, cli::Description{"longitude of take-off point"}, params_.longitude));
  flag(cli::make_flag(cli::Name{"max-altitude"}, cli::Description{"maximum altitude reached during flight"},
                      params_.max_altitude));
  flag(cli::make_flag(cli::Name{"aircraft-id"}, cli::Description{"id of aircraft that executes the flight"},
                      params_.aircraft_id));
  flag(cli::make_flag(cli::Name{"start-time"}, cli::Description{"planned start-time of flight"}, params_.start_time));
  flag(cli::make_flag(cli::Name{"end-time"}, cli::Description{"planned end-time of flight"}, params_.end_time));
  flag(cli::make_flag(cli::Name{"is-public"}, cli::Description{"mark the flight as publicly visible on dashboards"},
                      params_.is_public));
  flag(cli::make_flag(cli::Name{"give-digital-notice"}, cli::Description{"give digital notice to regulators"},
                      params_.give_digital_notice));
  flag(cli::make_flag(cli::Name{"buffer"}, cli::Description{"radius of flight zone centered around the take-off point"},
                      params_.buffer));
  flag(cli::make_flag(cli::Name{"geometry-file"}, cli::Description{"use the polygon defined in this geojson file"},
                      geometry_file_));

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

          if (geometry_file_) {
            std::ifstream in{geometry_file_.get()};
            Geometry geometry = json::parse(in);
            params_.geometry  = geometry;
          }

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Flights::CreateFlight::Result& result) {
            if (result)
              print_flight(ctxt.cout, result.value());
            else
              ctxt.cout << "Failed to create flight with id" << std::endl;
            context->stop();
          };

          if (!params_.geometry)
            client->flights().create_flight_by_point(params_, handler);
          else
            client->flights().create_flight_by_polygon(params_, handler);
        });

    context->run();

    return 0;
  });
}
