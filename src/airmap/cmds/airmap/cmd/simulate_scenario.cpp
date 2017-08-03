#include <airmap/cmds/airmap/cmd/simulate_scenario.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/scenario_simulator.h>
#include <airmap/util/telemetry_simulator.h>

#include <fstream>
#include <iterator>
#include <thread>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {
constexpr const char* component{"simulate-scenario-cli"};
}  // namespace

cmd::SimulateScenario::Collector::Collector(const util::Scenario& scenario) : scenario_{scenario} {
}

void cmd::SimulateScenario::Collector::collect_authentication_for_index(std::size_t index,
                                                                        const std::string& authentication) {
  scenario_.participants.at(index).authentication = authentication;
}

void cmd::SimulateScenario::Collector::collect_flight_id_for_index(std::size_t index, const Flight& flight) {
  scenario_.participants.at(index).flight = flight;
}

bool cmd::SimulateScenario::Collector::collect_key_for_index(std::size_t index, const std::string& key) {
  scenario_.participants.at(index).encryption_key = key;
  return scenario_.participants.size() == ++key_counter_;
}

const airmap::util::Scenario& cmd::SimulateScenario::Collector::scenario() const {
  return scenario_;
}

cmd::SimulateScenario::SimulateScenario()
    : cli::CommandWithFlagsAndAction{
          cli::Name{"simulate-scenario"},
          cli::Usage{"simulate multiple vehicles carrying out missions and submitting telemetry"},
          cli::Description{"simulate multiple vehicles carrying out missions and submitting telemetry"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"host"}, cli::Description{"telemetry host address"}, params_.host));
  flag(cli::make_flag(cli::Name{"port"}, cli::Description{"telemetry host port"}, params_.port));
  flag(cli::make_flag(cli::Name{"scenario-file"}, cli::Description{"use the scenario defined in this json file"},
                      params_.scenario_file));

  action([this](const cli::Command::Context& ctxt) {
    logger_ = create_default_logger(ctxt.cout);

    if (!params_.scenario_file) {
      logger_->error("missing parameter 'scenario-file'", component);
      return 1;
    }

    std::ifstream in{params_.scenario_file.get()};
    util::Scenario scenario = json::parse(in);

    collector_  = std::make_shared<Collector>(scenario);
    runner_     = std::make_shared<util::ScenarioSimulator::Runner>(5);
    auto result = ::airmap::Context::create(logger_);

    if (!result) {
      logger_->error("could not acquire resources for accessing AirMap services", component);
      return 1;
    }

    ::setenv("AIRMAP_TELEMETRY_HOST", params_.host.c_str(), 1);
    ::setenv("AIRMAP_TELEMETRY_PORT", boost::lexical_cast<std::string>(params_.port).c_str(), 1);

    context_ = result.value();

    context_->create_client_with_credentials(Client::Credentials{params_.api_key}, [this, &ctxt](const auto& result) {
      if (not result) {
        logger_->error("could not create client for accessing AirMap services", component);
        context_->stop();
        return;
      }
      logger_->info("successfully created client for AirMap services", component);

      client_ = result.value();

      for (std::size_t i = 0; i < collector_->scenario().participants.size(); i++) {
        client_->authenticator().authenticate_anonymously({collector_->scenario().participants.at(i).pilot.id}, [this, &ctxt, i](const auto& result) {
          if (not result) {
            logger_->error("could not authenticate with the Airmap services", component);
            context_->stop();
            return;
          }
          logger_->info("successfully authenticated with the AirMap services", component);

          Flights::CreateFlight::Parameters params;
          params.authorization = result.value().id;
          params.start_time    = Clock::universal_time();
          params.end_time      = Clock::universal_time() + Minutes{2};

          collector_->collect_authentication_for_index(i, result.value().id);
          const auto& participant = collector_->scenario().participants.at(i);
          const auto& polygon     = participant.geometry.details_for_polygon();

          params.aircraft_id = participant.aircraft.id;
          params.latitude    = polygon[0].coordinates[0].latitude;
          params.longitude   = polygon[0].coordinates[0].longitude;
          params.geometry    = participant.geometry;
          client_->flights().create_flight_by_polygon(params, [this, &ctxt, i](const auto& result) {
            if (!result) {
              try {
                std::rethrow_exception(result.error());
              } catch (const std::exception& e) {
                logger_->error(fmt::sprintf("could not create flight for polygon: %s", e.what()).c_str(), component);
              } catch (...) {
                logger_->error("could not create flight for polygon", component);
              }
              return;
            }
            logger_->info("successfully created flight for polygon", component);

            collector_->collect_flight_id_for_index(i, result.value());
            const auto& participant = collector_->scenario().participants.at(i);
            client_->flights().start_flight_communications(
                {participant.authentication.get(), participant.flight.get().id}, [this, &ctxt, i](const auto& result) {
                  if (!result) {
                    logger_->error("could not start flight comms", component);
                    context_->stop();
                    return;
                  }
                  logger_->info("successfully started flight comms", component);

                  if (collector_->collect_key_for_index(i, result.value().key)) {
                    auto simulator = std::make_shared<util::ScenarioSimulator>(collector_->scenario(), logger_);
                    runner_->start(simulator, client_);
                  }
                });
          });
        });
      }
    });

    context_->run();
    return 0;
  });
}