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

void cmd::SimulateScenario::Collector::collect_traffic_monitor_for_index(
    std::size_t index, const std::shared_ptr<Traffic::Monitor>& monitor) {
  scenario_.participants.at(index).monitor = monitor;
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
  flag(cli::make_flag(cli::Name{"version"}, cli::Description{"work against this version of the AirMap services"},
                      params_.version));
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"host"}, cli::Description{"telemetry host address"}, params_.host));
  flag(cli::make_flag(cli::Name{"port"}, cli::Description{"telemetry host port"}, params_.port));
  flag(cli::make_flag(cli::Name{"scenario-file"}, cli::Description{"use the scenario defined in this json file"},
                      params_.scenario_file));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_default_logger(ctxt.cout)};

    if (!params_.api_key) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!params_.api_key.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (params_.host && !params_.host.get().validate()) {
      log_.errorf(component, "parameter 'host' must not be empty");
      return 1;
    }

    if (!params_.scenario_file) {
      log_.errorf(component, "missing parameter 'scenario-file'");
      return 1;
    }

    std::ifstream in{params_.scenario_file.get()};
    if (!in) {
      log_.errorf(component, "failed to open %s for reading", params_.scenario_file.get());
      return 1;
    }
    util::Scenario scenario = json::parse(in);

    collector_  = std::make_shared<Collector>(scenario);
    runner_     = std::make_shared<util::ScenarioSimulator::Runner>(5);
    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "could not acquire resources for accessing AirMap services");
      return 1;
    }

    context_    = result.value();
    auto config = Client::default_configuration(params_.version, Client::Credentials{params_.api_key.get()});

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
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(config, [this, &ctxt](const auto& result) {
      if (not result) {
        log_.errorf(component, "could not create client for accessing AirMap services");
        context_->stop();
        return;
      }
      log_.infof(component, "successfully created client for AirMap services");

      client_ = result.value();

      for (std::size_t i = 0; i < collector_->scenario().participants.size(); i++) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
        client_->authenticator().authenticate_anonymously(
            {collector_->scenario().participants.at(i).pilot.id}, [this, &ctxt, i](const auto& result) {
              if (not result) {
                log_.errorf(component, "could not authenticate with the Airmap services");
                context_->stop();
                return;
              }
              log_.infof(component, "successfully authenticated with the AirMap services");

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
                    log_.errorf(component, "could not create flight for polygon: %s", e.what());
                  } catch (...) {
                    log_.errorf(component, "could not create flight for polygon");
                  }
                  context_->stop();
                  return;
                }
                log_.infof(component, "successfully created flight for polygon");

                collector_->collect_flight_id_for_index(i, result.value());
                const auto& participant = collector_->scenario().participants.at(i);

                client_->traffic().monitor({participant.flight.get().id, participant.authentication.get()}, [
                  this, i, id = participant.flight.get().id
                ](const auto& result) {
                  if (result) {
                    log_.infof(component, "successfully started monitoring traffic for flight: %s", id);
                    auto monitor = result.value();
                    monitor->subscribe(std::make_shared<Traffic::Monitor::LoggingSubscriber>(component, log_.logger()));
                    collector_->collect_traffic_monitor_for_index(i, monitor);
                  } else {
                    try {
                      std::rethrow_exception(result.error());
                    } catch (const std::exception& e) {
                      log_.errorf(component, "could not start monitoring traffic for flight %s: %s", id, e.what());
                    } catch (...) {
                      log_.errorf(component, "could not start monitoring traffic for flight %s", id);
                    }
                    context_->stop();
                  }
                });

                client_->flights().start_flight_communications(
                    {participant.authentication.get(), participant.flight.get().id},
                    [this, &ctxt, i](const auto& result) {
                      if (!result) {
                        log_.errorf(component, "could not start flight comms");
                        context_->stop();
                        return;
                      }
                      log_.infof(component, "successfully started flight comms");

                      if (collector_->collect_key_for_index(i, result.value().key)) {
                        auto simulator =
                            std::make_shared<util::ScenarioSimulator>(collector_->scenario(), log_.logger());
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
