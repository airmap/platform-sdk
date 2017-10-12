#include <airmap/cmds/airmap/cmd/query_rulesets.h>

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

constexpr const char* component{"query-rulesets"};

std::string print_ruleset(const airmap::RuleSet& r) {
  return fmt::sprintf(
    "    id:             %s\n"
    "    name:           %s\n"
    "    short_name:     %s\n"
    "    selection_type: %s\n"
    "    description:    %s\n"
    "    default:        %s\n"
    "    jurisdiction:\n"
    "       id:          %s\n"
    "       name:        %s\n"
    "       region:      %s\n"
    "    # rules:        %s\n",
    r.id, r.name, r.short_name, r.selection_type, r.description, r.is_default, r.jurisdiction.id, r.jurisdiction.name, r.jurisdiction.region, r.rules.size()
    // TBD - print rules
  );
}

std::string print_rulesets(const std::vector<airmap::RuleSet>& v) {
  std::ostringstream ss;
  for (const auto& r : v) {
    ss << print_ruleset(r);
  }
  return ss.str();
}

}  // namespace

cmd::QueryRulesets::QueryRulesets()
    : cli::CommandWithFlagsAndAction{"query-rulesets", "queries rulesets by geometry or id with the AirMap services",
                                     "queries rulesets by geometry or id with the AirMap services"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(cli::make_flag("geometry-file", "use the polygon defined in this geojson file", geometry_file_));
  flag(cli::make_flag("ruleset-id", "id of ruleset", ruleset_id_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cout))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    if (!ruleset_id_ && !geometry_file_) {
      log_.errorf(component, "missing parameter 'ruleset-id' or 'geometry-file'");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    context_ = result.value();
    auto config  = Client::load_configuration_from_json(in_config);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(
        config, [this](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            try {
              std::rethrow_exception(result.error());
            } catch (const std::exception& e) {
              log_.errorf(component, "failed to create client: %s", e.what());
            } catch (...) {
              log_.errorf(component, "failed to create client");
            }
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();

          if (ruleset_id_) {
            RuleSets::ForId::Parameters params;
            params.id = ruleset_id_.get();
            client_->rulesets().for_id(params, std::bind(&QueryRulesets::handle_ruleset_for_id_result, this, std::placeholders::_1));
          } else if (geometry_file_) {
            std::ifstream in{geometry_file_.get()};
            if (!in) {
              log_.errorf(component, "failed to open %s for reading", geometry_file_.get());
              return;
            }
            Geometry geometry = json::parse(in);
            RuleSets::Search::Parameters params;
            params.geometry = geometry;
            client_->rulesets().search(params, std::bind(&QueryRulesets::handle_ruleset_search_result, this, std::placeholders::_1));
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

void cmd::QueryRulesets::handle_ruleset_for_id_result(const RuleSets::ForId::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried ruleset from ruleset-id:\n%s", print_ruleset(result.value()));
    context_->stop();
} else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query for rulesets: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query for rulesets");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
    return;
  }
}

void cmd::QueryRulesets::handle_ruleset_search_result(const RuleSets::Search::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried rulesets from geometry:\n%s", print_rulesets(result.value()));
    context_->stop();
} else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query for rulesets: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query for rulesets");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
    return;
  }
}
