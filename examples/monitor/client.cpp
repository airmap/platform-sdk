// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <airmap/context.h>
#include <airmap/monitor/client.h>
#include <airmap/util/formatting_logger.h>

#include <signal.h>
#include <string.h>

#include <iostream>

namespace {

constexpr const char* component{"monitor example"};

/// LoggingReceiver implements handling of updates originating from AirMap's monitor daemon.
struct LoggingReceiver : public airmap::monitor::Client::UpdateStream::Receiver {
  /// LoggingReceiver initializes a new instance with 'logger'.
  explicit LoggingReceiver(const std::shared_ptr<airmap::Logger>& logger) : log{logger} {
  }

  // From Receiver
  void handle_update(const airmap::monitor::Client::Update& update) override {
    auto record = log.info("LoggingReceiver");

    record.printf(
        "received update\n"
        "  traffic: %d updates\n",
        update.traffic.size());

    for (const auto& tu : update.traffic) {
      record.printf(
          "    track id:    %s\n"
          "    aircraft id: %s\n",
          tu.id, tu.aircraft_id);
    }
  }

  airmap::util::FormattingLogger log;
};

}  // namespace

/// usage: monitor.client [ENDPOINT] [TEST]
int main(int argc, char** argv) {
  bool run_under_testing = false;
  std::string endpoint   = "unix:/tmp/airmap/monitor.socket";

  if (argc >= 2) {
    endpoint = argv[1];
  }

  if (argc >= 3) {
    run_under_testing = argv[2] == std::string{"test"};
  }

  auto logger   = ::airmap::create_default_logger(std::cout);
  auto log      = ::airmap::util::FormattingLogger{logger};
  auto result   = ::airmap::Context::create(logger);
  auto receiver = std::make_shared<LoggingReceiver>(logger);
  auto client   = std::shared_ptr<::airmap::monitor::Client>{};
  auto stream   = std::shared_ptr<::airmap::monitor::Client::UpdateStream>{};

  if (!result) {
    log.errorf(component, "failed to create AirMap context: %s", result.error());
    return 1;
  }

  log.infof(component, "connecting to monitor on endpoint %s", endpoint);

  auto context = result.value();

  ::airmap::monitor::Client::Configuration config;
  config.endpoint = endpoint;
  config.logger   = logger;

  if (run_under_testing) {
    context->schedule_in(::airmap::microseconds(5 * 1000 * 1000), [&]() {
      log.infof(component, "stopping execution");
      context->stop();
    });
  }

  context->create_monitor_client_with_configuration(config, [&](const auto& result) {
    if (result) {
      log.infof(component, "successfully created monitor client");
      client = result.value();
      client->connect_to_updates([&](const auto& result) {
        if (result) {
          log.infof(component, "successfully connected to updates");
          stream = result.value();
          stream->subscribe(receiver);
        } else {
          log.errorf(component, "failed to connect to updates: %s", result.error());
          context->stop(airmap::Context::ReturnCode::error);
        }
      });
    } else {
      log.errorf(component, "failed to create monitor client: %s", result.error());
      context->stop(airmap::Context::ReturnCode::error);
    }
  });

  auto rc = context->exec({SIGINT, SIGQUIT}, [&](int sig) {
    log.infof(component, "received [%s], shutting down", ::strsignal(sig));

    if (stream) {
      stream->unsubscribe(receiver);
    }

    context->stop();
  });

  return rc == airmap::Context::ReturnCode::success;
}
