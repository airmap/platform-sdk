/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  airmap.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/add_aircraft.h>
#include <airmap/cmds/airmap/cmd/aircraft_models.h>
#include <airmap/cmds/airmap/cmd/create_flight.h>
#include <airmap/cmds/airmap/cmd/end_flight.h>
#include <airmap/cmds/airmap/cmd/evaluate_rulesets.h>
#include <airmap/cmds/airmap/cmd/fetch_rules.h>
#include <airmap/cmds/airmap/cmd/get_advisories.h>
#include <airmap/cmds/airmap/cmd/get_status.h>
#include <airmap/cmds/airmap/cmd/init.h>
#include <airmap/cmds/airmap/cmd/login.h>
#include <airmap/cmds/airmap/cmd/monitor_mids.h>
#include <airmap/cmds/airmap/cmd/monitor_traffic.h>
#include <airmap/cmds/airmap/cmd/pilot.h>
#include <airmap/cmds/airmap/cmd/plan_flight.h>
#include <airmap/cmds/airmap/cmd/query_rulesets.h>
#include <airmap/cmds/airmap/cmd/render_briefing.h>
#include <airmap/cmds/airmap/cmd/report_weather.h>
#include <airmap/cmds/airmap/cmd/search_airspace.h>
#include <airmap/cmds/airmap/cmd/simulate_scenario.h>
#include <airmap/cmds/airmap/cmd/simulate_telemetry.h>
#include <airmap/cmds/airmap/cmd/start_flight_comms.h>
#include <airmap/cmds/airmap/cmd/submit_flight.h>
#include <airmap/cmds/airmap/cmd/test.h>
#include <airmap/cmds/airmap/cmd/version.h>

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
    cmd_.command(std::make_shared<cmd::AddAircraft>());
    cmd_.command(std::make_shared<cmd::AircraftModels>());
    cmd_.command(std::make_shared<cmd::CreateFlight>());
    cmd_.command(std::make_shared<cmd::EndFlight>());
    cmd_.command(std::make_shared<cmd::FetchRules>());
    cmd_.command(std::make_shared<cmd::GetAdvisories>());
    cmd_.command(std::make_shared<cmd::GetStatus>());
    cmd_.command(std::make_shared<cmd::EvaluateRuleSets>());
    cmd_.command(std::make_shared<cmd::Init>());
    cmd_.command(std::make_shared<cmd::Login>());
    cmd_.command(std::make_shared<cmd::MonitorMids>());
    cmd_.command(std::make_shared<cmd::MonitorTraffic>());
    cmd_.command(std::make_shared<cmd::Pilot>());
    cmd_.command(std::make_shared<cmd::PlanFlight>());
    cmd_.command(std::make_shared<cmd::QueryRuleSets>());
    cmd_.command(std::make_shared<cmd::RenderBriefing>());
    cmd_.command(std::make_shared<cmd::ReportWeather>());
    cmd_.command(std::make_shared<cmd::SearchAirspace>());
    cmd_.command(std::make_shared<cmd::StartFlightComms>());
    cmd_.command(std::make_shared<cmd::SimulateScenario>());
    cmd_.command(std::make_shared<cmd::SimulateTelemetry>());
    cmd_.command(std::make_shared<cmd::SubmitFlight>());
    cmd_.command(std::make_shared<cmd::Test>());
    cmd_.command(std::make_shared<cmd::Version>());

    add_conditional_subcommands();
  }

  int run(const std::vector<std::string>& args) {
    return cmd_.run(cli::Command::Context{std::cin, std::cout, std::cerr, args});
  }

 private:
  // add_conditional_subcommands adds subcommands that depend on
  // compile-time parameters.
  void add_conditional_subcommands();

  cli::CommandWithSubcommands cmd_;
};

}  // namespace

#if defined(AIRMAP_ENABLE_GRPC)
#include <airmap/cmds/airmap/cmd/daemon.h>

void Airmap::add_conditional_subcommands() {
  cmd_.command(std::make_shared<cmd::Daemon>());
}
#else   // AIRMAP_ENABLE_GRPC
void Airmap::add_conditional_subcommands() {
}
#endif  // AIRMAP_ENABLE_GRPC

int main(int argc, char** argv) {
  Airmap airmap;
  return airmap.run(cli::args(argc, argv));
}
