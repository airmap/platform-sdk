#include <airmap/qt/client.h>

#include <airmap/qt/dispatcher.h>

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
  ~Private() {
    context_runner_->stop();
  }

  Client::Configuration configuration_;
  std::shared_ptr<ContextRunner> context_runner_;
  std::shared_ptr<airmap::Client> client_;
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
              cb(CreateResult{
                  new Client{std::unique_ptr<Private>{new Private{configuration, cr, result.value()}}, parent}});
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
  throw std::logic_error{"not implemented"};
}

airmap::Aircrafts& airmap::qt::Client::aircrafts() {
  throw std::logic_error{"not implemented"};
}

airmap::Airspaces& airmap::qt::Client::airspaces() {
  throw std::logic_error{"not implemented"};
}

airmap::FlightPlans& airmap::qt::Client::flight_plans() {
  throw std::logic_error{"not implemented"};
}

airmap::Flights& airmap::qt::Client::flights() {
  throw std::logic_error{"not implemented"};
}

airmap::Pilots& airmap::qt::Client::pilots() {
  throw std::logic_error{"not implemented"};
}

airmap::RuleSets& airmap::qt::Client::rulesets() {
  throw std::logic_error{"not implemented"};
}

airmap::Status& airmap::qt::Client::status() {
  throw std::logic_error{"not implemented"};
}

airmap::Telemetry& airmap::qt::Client::telemetry() {
  throw std::logic_error{"not implemented"};
}

airmap::Traffic& airmap::qt::Client::traffic() {
  throw std::logic_error{"not implemented"};
}
