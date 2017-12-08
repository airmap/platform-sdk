#include <airmap/cmds/airmap/cmd/evaluate_rulesets.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/paths.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

constexpr const char* component{"evaluate-rulesets"};

void print_evaluation(std::ostream& out, const airmap::Evaluation& e) {
  cli::TabWriter tw;

  tw << "# rulesets"
     << "# validations"
     << "# authorizations"
     << "# failures" << cli::TabWriter::NewLine{} << e.rulesets.size() << e.validations.size()
     << e.authorizations.size() << e.failures.size();

  tw.flush(out);
}

}  // namespace

cmd::EvaluateRuleSets::EvaluateRuleSets()
    : cli::CommandWithFlagsAndAction{"evaluate-rulesets",
                                     "evalutes rulesets and return a list of rules matching results",
                                     "evalutes rulesets and return a list of rules matching results"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(cli::make_flag("evaluation-file", "evaluation-file", evaluation_file_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cerr))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    if (!evaluation_file_ || !evaluation_file_.get().validate()) {
      log_.errorf(component, "missing parameter 'evaluation-file'");
      return 1;
    }

    std::ifstream evaluation_in{evaluation_file_.get()};
    if (!evaluation_in) {
      log_.errorf(component, "failed to open %s for reading", evaluation_file_.get());
      return 1;
    }
    params_ = json::parse(evaluation_in);

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto config  = Client::load_configuration_from_json(in_config);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const RuleSets::EvaluateRules::Result& result) {
            if (result) {
              log_.infof(component, "successfully evaluated rulesets");
              print_evaluation(ctxt.cout, result.value());
              context->stop();
            } else {
              log_.errorf(component, "failed to evaluate rulesets: %s", result.error());
              context->stop(::airmap::Context::ReturnCode::error);
              return;
            }
          };

          client->rulesets().evaluate_rulesets(params_, handler);
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
