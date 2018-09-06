//
//  monitor_mids.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/monitor_mids.h>

#include <airmap/client.h>
#include <airmap/context.h>

#include <airmap/boost/context.h>
#include <airmap/net/mqtt/boost/broker.h>
#include <airmap/util/fmt.h>

#include <airmap/pregenerated/amd_uav_flight_update.pb.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;
namespace fmt = airmap::util::fmt;

namespace {
constexpr const char* component{"monitor-mids"};
}

cmd::MonitorMids::MonitorMids()
    : cli::CommandWithFlagsAndAction{"monitor-mids", "monitors traffic visible to mids",
                                     "monitors traffic visible to mids"} {
  flag(flags::log_level(params_.log_level));
  flag(cli::make_flag("mids", "comma-separated list of mids", params_.mids));
  flag(cli::make_flag("mqtt-host", "host address of the mqtt broker", params_.mqtt.host));
  flag(cli::make_flag("mqtt-port", "port of the mqtt broker", params_.mqtt.port));
  flag(cli::make_flag("mqtt-username", "username for accessing the mqtt broker", params_.mqtt.username));
  flag(cli::make_flag("mqtt-password", "password for accessing the mqtt broker", params_.mqtt.password));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(params_.log_level, create_default_logger(ctxt.cerr))};

    if (!params_.mqtt.host || !params_.mqtt.host.get().validate()) {
      log_.errorf(component, "missing parameter 'mqtt-host'");
      return 1;
    }

    if (!params_.mqtt.port) {
      log_.errorf(component, "missing parameter 'mqtt-port'");
      return 1;
    }

    if (!params_.mqtt.username || !params_.mqtt.username.get().validate()) {
      log_.errorf(component, "missing parameter 'mqtt-username'");
      return 1;
    }

    if (!params_.mqtt.password || !params_.mqtt.password.get().validate()) {
      log_.errorf(component, "missing parameter 'mqtt-password'");
      return 1;
    }

    log_.infof(component,
               "configuration:\n"
               "  mids:          %s\n"
               "  mqtt.host:     %s\n"
               "  mqtt.port:     %d\n"
               "  mqtt.username: %s\n"
               "  mqtt.password: %s",
               params_.mids.get(), params_.mqtt.host.get(), params_.mqtt.port.get(), params_.mqtt.username.get(),
               params_.mqtt.password.get());

    auto bc  = boost::Context::create(log_.logger());
    context_ = bc;
    broker_  = std::make_shared<net::mqtt::boost::Broker>(params_.mqtt.host.get(), params_.mqtt.port.get(),
                                                         log_.logger(), bc->io_service());
    broker_->connect({params_.mqtt.username.get(), params_.mqtt.password.get()}, [this](const auto& result) {
      if (result) {
        client_ = result.value();

        std::vector<std::string> mids;
        ::boost::algorithm::split(mids, params_.mids.get().string(), ::boost::algorithm::is_any_of(","));

        auto handler = [this](const std::string& topic, const std::string& message) {
          // TODO(tvoss): We should not have to identify the scope by interpreting the topic name here.
          // Ideally, the update would give us all information required for classification and further processing.
          auto scope = topic.substr(0, topic.find_first_of('/'));

          ::airmap::telemetry::AmdUavFlightUpdates updates;
          if (updates.ParseFromString(message)) {
            for (int i = 0; i < updates.updates_size(); i++) {
              const auto& update = updates.updates(i);
              log_.infof(component,
                         "traffic reported:\n"
                         "  scope:         %s\n"
                         "  flight-id:     %s\n"
                         "  latitude [°]:  %f\n"
                         "  longitude [°]: %f\n"
                         "  course [°]:    %f\n"
                         "  altitude [m]:  %f\n"
                         "  speed [m/s]:   %f",
                         scope, update.flight_id(), update.latitude(), update.longitude(), update.course(),
                         update.altitude(), update.speed());
            }
          }
        };

        for (const auto& mid : mids) {
          subscriptions_.insert(client_->subscribe(fmt::sprintf("mav/telemetry/position/%s", mid),
                                                   net::mqtt::QualityOfService::exactly_once, handler));
          subscriptions_.insert(client_->subscribe(fmt::sprintf("uav/telemetry/position/%s", mid),
                                                   net::mqtt::QualityOfService::exactly_once, handler));
        }
      } else {
        log_.errorf(component, "failed to connect to mqtt broker: %s", result.error());
        context_->stop(::airmap::Context::ReturnCode::error);
        return;
      }
    });

    return context_->exec({SIGINT, SIGQUIT},
                          [this](int sig) {
                            log_.infof(component, "received [%s], shutting down", ::strsignal(sig));
                            context_->stop();
                          }) == ::airmap::Context::ReturnCode::success
               ? 0
               : 1;
  });
}
