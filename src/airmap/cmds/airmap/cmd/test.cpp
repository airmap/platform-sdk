//
//  test.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/cmds/airmap/cmd/test.h>

#include <airmap/paths.h>

#include <airmap/cmds/airmap/cmd/test/laanc.phoenix.h>
#include <airmap/util/formatting_logger.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"test"};
}  // namespace

cmd::Test::Test() : cli::CommandWithFlagsAndAction{"test", "executes runtime tests", "executes runtime tests"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::token_file(token_file_));
  flag(cli::make_flag("test-suite", "test suite name", test_suite_));

  test_suite_registry_[test::laanc::PhoenixZoo::name]    = []() { return std::make_shared<test::laanc::PhoenixZoo>(); };
  test_suite_registry_[test::laanc::PhoenixManual::name] = []() {
    return std::make_shared<test::laanc::PhoenixManual>();
  };
  test_suite_registry_[test::laanc::PhoenixSchwegg::name] = []() {
    return std::make_shared<test::laanc::PhoenixSchwegg>();
  };
  test_suite_registry_[test::laanc::PhoenixUniversity::name] = []() {
    return std::make_shared<test::laanc::PhoenixUniversity>();
  };
  test_suite_registry_[test::laanc::KentuckyFlorence::name] = []() {
    return std::make_shared<test::laanc::KentuckyFlorence>();
  };
  test_suite_registry_[test::laanc::NevadaReno::name] = []() { return std::make_shared<test::laanc::NevadaReno>(); };
  test_suite_registry_[test::laanc::ArkansasPineBluff::name] = []() {
    return std::make_shared<test::laanc::ArkansasPineBluff>();
  };
  test_suite_registry_[test::laanc::WyomingTetonVillage::name] = []() {
    return std::make_shared<test::laanc::WyomingTetonVillage>();
  };

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger(create_filtering_logger(log_level_, create_default_logger(ctxt.cerr)));

    if (!test_suite_ || !test_suite_.get().validate()) {
      auto record = log_.error(component);

      record.print("missing parameter 'test-suite', choose from:\n");
      for (const auto& pair : test_suite_registry_) {
        record << "  " << pair.first << "\n";
      }

      return 1;
    }

    auto it = test_suite_registry_.find(test_suite_.get());
    if (it == test_suite_registry_.end()) {
      log_.errorf(component, "failed to identify test suite %s", test_suite_.get());
      return 1;
    }

    auto suite = it->second();

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    if (!token_file_) {
      token_file_ = TokenFile{paths::token_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    auto config = Client::load_configuration_from_json(in_config);

    std::ifstream in_token{token_file_.get()};
    if (!in_token) {
      log_.errorf(component, "failed to open token file %s for reading", token_file_);
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto token   = Token::load_from_json(in_token);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, suite, config, token, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          suite->run(log_.logger(), result.value(), context, token);
        });

    return context->exec({SIGINT, SIGQUIT},
                         [this, context](int sig) {
                           log_.infof(component, "received [%s], shutting down", ::strsignal(sig));
                           context->stop();
                         }) == ::airmap::Context::ReturnCode::success
               ? 0
               : 1;
  });
}
