#include <airmap/cmds/airmap/cmd/authorize_anonymous.h>
#include <airmap/cmds/airmap/cmd/authorize_password.h>
#include <airmap/cmds/airmap/cmd/authorize_refresh.h>
#include <airmap/cmds/airmap/cmd/create_flight.h>
#include <airmap/cmds/airmap/cmd/daemon.h>
#include <airmap/cmds/airmap/cmd/get_status.h>
#include <airmap/cmds/airmap/cmd/login.h>
#include <airmap/cmds/airmap/cmd/monitor_traffic.h>
#include <airmap/cmds/airmap/cmd/pilot.h>
#include <airmap/cmds/airmap/cmd/simulate_scenario.h>
#include <airmap/cmds/airmap/cmd/simulate_telemetry.h>
#include <airmap/cmds/airmap/cmd/start_flight_comms.h>

#include <airmap/do_not_copy_or_move.h>
#include <airmap/util/cli.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {

class Airmap : airmap::DoNotCopyOrMove {
 public:
  Airmap()
      : cmd_{cli::Name{"airmap"}, cli::Usage{"interacts with AirMap services"},
             cli::Description{"interacts with AirMap services"}} {
    cmd_.command(std::make_shared<cmd::AuthorizePassword>());
    cmd_.command(std::make_shared<cmd::AuthorizeAnonymous>());
    cmd_.command(std::make_shared<cmd::AuthorizeRefresh>());
    cmd_.command(std::make_shared<cmd::CreateFlight>());
    cmd_.command(std::make_shared<cmd::Daemon>());
    cmd_.command(std::make_shared<cmd::GetStatus>());
    cmd_.command(std::make_shared<cmd::Login>());
    cmd_.command(std::make_shared<cmd::MonitorTraffic>());
    cmd_.command(std::make_shared<cmd::Pilot>());
    cmd_.command(std::make_shared<cmd::StartFlightComms>());
    cmd_.command(std::make_shared<cmd::SimulateScenario>());
    cmd_.command(std::make_shared<cmd::SimulateTelemetry>());
  }

  int run(const std::vector<std::string>& args) {
    return cmd_.run(cli::Command::Context{std::cin, std::cout, args});
  }

 private:
  cli::CommandWithSubcommands cmd_;
};

}  // namespace

int main(int argc, char** argv) {
  Airmap airmap;
  return airmap.run(cli::args(argc, argv));
}
