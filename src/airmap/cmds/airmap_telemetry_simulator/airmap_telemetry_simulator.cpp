#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/flight.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>

#include <airmap/util/cli.h>
#include <airmap/util/telemetry_simulator.h>

#include <boost/lexical_cast.hpp>

#include <stdlib.h>

#include <cmath>

#include <chrono>
#include <iostream>
#include <thread>

namespace cli = airmap::util::cli;

namespace {

std::vector<std::vector<double>> coordinates = {
    {6.165432929992676, 46.20978927893167},  {6.165958642959595, 46.2098783736693},
    {6.16581916809082, 46.21030899619723},   {6.166441440582275, 46.21005656271103},
    {6.167149543762206, 46.21040551457655},  {6.166784763336182, 46.210806435259656},
    {6.167525053024292, 46.21067279535712},  {6.16782546043396, 46.21079158639765},
    {6.16732120513916, 46.21129644545361},   {6.167728900909424, 46.211318718540376},
    {6.167804002761841, 46.211548873241355}, {6.168426275253296, 46.210977196884144},
    {6.168748140335083, 46.211192504262655}, {6.168630123138427, 46.21154144891121},
    {6.169005632400513, 46.21128902108935},  {6.169424057006836, 46.21151917591475},
    {6.169016361236572, 46.211972058400264}, {6.1693596839904785, 46.21217251339009},
    {6.168909072875977, 46.212484330808905}, {6.168297529220581, 46.21229130118695},
    {6.16905927658081, 46.211556297570475},  {6.168007850646973, 46.21216508914424},
    {6.168351173400879, 46.211222201765835}, {6.167621612548827, 46.21196463412732},
    {6.167471408843994, 46.21154144891121},  {6.166752576828003, 46.21163054080662},
    {6.167407035827637, 46.210880679509444}, {6.166602373123169, 46.21104401650574},
    {6.166870594024658, 46.21128159672409},  {6.16658091545105, 46.211556297570475},
    {6.16606593132019, 46.211318718540376},  {6.16682767868042, 46.210457485941305},
    {6.166613101959228, 46.21036096765333},  {6.165754795074463, 46.21121477739155},
    {6.16557240486145, 46.211133109208035},  {6.166430711746215, 46.21026444919574},
    {6.165196895599364, 46.21095492365889},  {6.165432929992676, 46.20978927893167}};

std::vector<airmap::Geometry::Coordinate> init_polygon() {
  std::vector<airmap::Geometry::Coordinate> coords;
  for (const auto& v : coordinates) {
    coords.push_back(airmap::Geometry::Coordinate{v[1], v[0], airmap::Optional<double>{}, airmap::Optional<double>{}});
  }
  return coords;
}

auto polygon = airmap::Geometry::polygon(init_polygon());

struct Params {
  Params()
      : host{"127.0.0.1"},
        port{16060},
        api_key{"not-an-api-key"},
        encryption_key{"MDAwMDExMTEyMjIyMzMzMzQ0NDQ1NTU1NjY2Njc3Nzc="} {
    flight.id = "flight|J4G8KLPhm50WW3cMQ2KpniB5GvAg";
  }
  std::string host;
  std::uint16_t port{16060};
  std::uint16_t frequency{5};
  airmap::Flight flight;
  std::string api_key;
  std::string encryption_key;
} params;

}  // namespace

int main(int argc, char** argv) {
  cli::CommandWithFlagsAndAction cmd{cli::Name{"airmap-telemetry-simulator"},
                                     cli::Usage{"simulates telemetry submission"},
                                     cli::Description{"simulates telemetry submission"}};

  cmd.flag(cli::make_flag(cli::Name{"host"}, cli::Description{"telemetry host address"}, params.host))
      .flag(cli::make_flag(cli::Name{"port"}, cli::Description{"telemetry host port"}, params.port))
      .flag(cli::make_flag(cli::Name{"frequency"}, cli::Description{"telemetry is sent with `FREQUENCY` Hz"},
                           params.frequency))
      .flag(cli::make_flag(cli::Name{"flight-id"}, cli::Description{"telemetry is sent for this flight id"},
                           params.flight.id))
      .flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"key used to authenticate access to AirMap APIs"},
                           params.api_key))
      .flag(cli::make_flag(cli::Name{"enc-key"}, cli::Description{"telemetry is encrypted with this key"},
                           params.encryption_key));

  cmd.action([](const cli::Command::Context& ctxt) {
    auto result = airmap::Context::create(airmap::create_default_logger(ctxt.cout));

    if (!result) {
      ctxt.cout << "Could not acquire resources for accessing AirMap services" << std::endl;
      return 1;
    }

    auto context = result.value();

    ctxt.cout << "Sending telemetry package to" << std::endl
              << "  host:      " << params.host << std::endl
              << "  port:      " << params.port << std::endl
              << "  frequency: " << params.frequency << std::endl
              << "  flight-id: " << params.flight.id << std::endl
              << "  api-key:   " << params.api_key << std::endl
              << "  enc-key:   " << params.encryption_key << std::endl;

    ::setenv("AIRMAP_TELEMETRY_HOST", params.host.c_str(), 1);
    ::setenv("AIRMAP_TELEMETRY_PORT", boost::lexical_cast<std::string>(params.port).c_str(), 1);

    auto credentials    = airmap::Credentials{};
    credentials.api_key = params.api_key;

    context->create_client_with_configuration(
        airmap::Client::default_production_configuration(credentials),
        [](const airmap::Context::ClientCreateResult& result) {
          if (!result) {
            return;
          }

          auto client = result.value();

          std::thread submitter{[client]() {
            airmap::util::TelemetrySimulator simulator{polygon.details_for_polygon()};

            while (true) {
              auto data = simulator.update();

              std::cout << data.latitude << ", " << data.longitude << std::endl;
              client->telemetry().submit_updates(params.flight, params.encryption_key,
                                                 {airmap::Telemetry::Update{airmap::Telemetry::Position{
                                                     airmap::milliseconds_since_epoch(airmap::Clock::universal_time()),
                                                     data.latitude, data.longitude, 100, 100, 2}}});
              std::this_thread::sleep_for(std::chrono::milliseconds{1000 / params.frequency});
            }
          }};

          submitter.detach();
        });

    return context->run() == airmap::Context::ReturnCode::success ? 0 : 1;
  });

  return cmd.run(cli::Command::Context{std::cin, std::cout, cli::args(argc, argv)});
}
