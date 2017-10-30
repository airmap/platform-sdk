#include <airmap/qt/client.h>

#include <airmap/qt/dispatcher.h>

#include <airmap/qt/aircrafts.h>
#include <airmap/qt/airspaces.h>
#include <airmap/qt/authenticator.h>
#include <airmap/qt/flight_plans.h>
#include <airmap/qt/flights.h>
#include <airmap/qt/pilots.h>
#include <airmap/qt/status.h>

#include <memory>
#include <thread>

namespace {

class ContextRunner {
 public:
  explicit ContextRunner(const std::shared_ptr<airmap::Context>& context) : context_{context} {
  }

  void start() {
    worker_ = std::thread{[this]() { context_->run(); }};
  }

  void stop() {
    context_->stop();
    if (worker_.joinable())
      worker_.join();
  }

  const std::shared_ptr<airmap::Context>& context() const {
    return context_;
  }

 private:
  std::shared_ptr<airmap::Context> context_;
  std::thread worker_;
};

}  // namespace

struct airmap::qt::Client::Private {
  explicit Private(const Client::Configuration& configuration, const std::shared_ptr<ContextRunner>& context_runner,
                   const std::shared_ptr<Dispatcher>& dispatcher, const std::shared_ptr<airmap::Client>& client)
      : configuration_{configuration},
        context_runner_{context_runner},
        dispatcher_{dispatcher},
        client_{client},
        aircrafts_{new airmap::qt::Aircrafts{dispatcher_, client_}},
        airspaces_{new airmap::qt::Airspaces{dispatcher_, client_}},
        authenticator_{new airmap::qt::Authenticator{dispatcher_, client_}},
        flight_plans_{new airmap::qt::FlightPlans{dispatcher_, client_}},
        flights_{new airmap::qt::Flights{dispatcher_, client_}},
        pilots_{new airmap::qt::Pilots{dispatcher_, client_}},
        status_{new airmap::qt::Status{dispatcher_, client_}} {
  }

  ~Private() {
    context_runner_->stop();
  }

  Client::Configuration configuration_;
  std::shared_ptr<ContextRunner> context_runner_;
  std::shared_ptr<airmap::qt::Dispatcher> dispatcher_;
  std::shared_ptr<airmap::Client> client_;
  std::shared_ptr<airmap::qt::Aircrafts> aircrafts_;
  std::shared_ptr<airmap::qt::Airspaces> airspaces_;
  std::shared_ptr<airmap::qt::Authenticator> authenticator_;
  std::shared_ptr<airmap::qt::FlightPlans> flight_plans_;
  std::shared_ptr<airmap::qt::Flights> flights_;
  std::shared_ptr<airmap::qt::Pilots> pilots_;
  std::shared_ptr<airmap::qt::Status> status_;
};

void airmap::qt::Client::create(const Client::Configuration& configuration, const std::shared_ptr<Logger>& logger,
                                QObject* parent, const CreateCallback& cb) {
  auto result     = Context::create(logger);
  auto dispatcher = std::make_shared<Dispatcher>(result.value());

  if (!result) {
    dispatcher->dispatch_to_qt([result, cb]() { cb(CreateResult{result.error()}); });
  } else {
    auto cr = std::make_shared<ContextRunner>(result.value());

    cr->context()->create_client_with_configuration(
        configuration, [dispatcher, configuration, parent, cr, cb](const auto& result) {
          dispatcher->dispatch_to_qt([dispatcher, configuration, parent, cr, result, cb]() {
            if (result) {
              cb(CreateResult{new Client{
                  std::unique_ptr<Private>{new Private{configuration, cr, dispatcher, result.value()}}, parent}});
            } else {
              cb(CreateResult{result.error()});
            }
          });
        });

    cr->start();
  }
}

airmap::qt::Client::Client(std::unique_ptr<Private>&& d, QObject* parent) : QObject{parent}, d_{std::move(d)} {
}

// From airmap::Client
airmap::Authenticator& airmap::qt::Client::authenticator() {
  return *d_->authenticator_;
}

airmap::Aircrafts& airmap::qt::Client::aircrafts() {
  return *d_->aircrafts_;
}

airmap::Airspaces& airmap::qt::Client::airspaces() {
  return *d_->airspaces_;
}

airmap::FlightPlans& airmap::qt::Client::flight_plans() {
  return *d_->flight_plans_;
}

airmap::Flights& airmap::qt::Client::flights() {
  return *d_->flights_;
}

airmap::Pilots& airmap::qt::Client::pilots() {
  throw std::logic_error{"not implemented"};
}

airmap::RuleSets& airmap::qt::Client::rulesets() {
  throw std::logic_error{"not implemented"};
}

airmap::Status& airmap::qt::Client::status() {
  return *d_->status_;
}

airmap::Telemetry& airmap::qt::Client::telemetry() {
  throw std::logic_error{"not implemented"};
}

airmap::Traffic& airmap::qt::Client::traffic() {
  throw std::logic_error{"not implemented"};
}
