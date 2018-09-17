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

void print_ruleset(std::ostream& out, const airmap::RuleSet& r) {
  cli::TabWriter tw;

  tw << "id"
     << "name"
     << "short-name"
     << "selection-type"
     << "description"
     << "default"
     << "jurisdiction-id"
     << "jurisdiction-name"
     << "jurisdiction-region";
  tw << cli::TabWriter::NewLine{};
  tw << r.id << r.name << r.short_name << r.selection_type << r.description << r.is_default << r.jurisdiction.id
     << r.jurisdiction.name << r.jurisdiction.region;

  tw.flush(out);
}

void print_rulesets(std::ostream& out, const std::vector<airmap::RuleSet>& v) {
  cli::TabWriter tw;

  tw << "id"
     << "name"
     << "short-name"
     << "selection-type"
     << "description"
     << "default"
     << "jurisdiction-id"
     << "jurisdiction-name"
     << "jurisdiction-region";

  for (const auto& r : v) {
    tw << cli::TabWriter::NewLine{};
    tw << r.id << r.name << r.short_name << r.selection_type << r.description << r.is_default << r.jurisdiction.id
       << r.jurisdiction.name << r.jurisdiction.region;
  }

  tw.flush(out);
}

}  // namespace

cmd::QueryRuleSets::QueryRuleSets()
    : cli::CommandWithFlagsAndAction{"query-rulesets", "queries rulesets by geometry or id from the AirMap services",
                                     "queries rulesets by geometry or id from the AirMap services"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(cli::make_flag("geometry-file", "use the polygon defined in this geojson file", geometry_file_));
  flag(cli::make_flag("ruleset-id", "id of ruleset", ruleset_id_));

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

    if (!ruleset_id_ && !geometry_file_) {
      log_.errorf(component, "missing parameter 'ruleset-id' or 'geometry-file'");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    context_    = result.value();
    auto config = Client::load_configuration_from_json(in_config);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(
        config, [this, &ctxt](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context_->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          client_ = result.value();

          if (ruleset_id_) {
            RuleSets::ForId::Parameters params;
            params.id = ruleset_id_.get();
            client_->rulesets().for_id(params, std::bind(&QueryRuleSets::handle_ruleset_for_id_result, this,
                                                         std::placeholders::_1, std::ref(ctxt)));
          } else if (geometry_file_) {
            std::ifstream in{geometry_file_.get()};
            if (!in) {
              log_.errorf(component, "failed to open %s for reading", geometry_file_.get());
              return;
            }
            Geometry geometry = json::parse(in);
            RuleSets::Search::Parameters params;
            params.geometry = geometry;
            client_->rulesets().search(params, std::bind(&QueryRuleSets::handle_ruleset_search_result, this,
                                                         std::placeholders::_1, std::ref(ctxt)));
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

void cmd::QueryRuleSets::handle_ruleset_for_id_result(const RuleSets::ForId::Result& result, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully queried ruleset");
    print_ruleset(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to query for rulesets: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::QueryRuleSets::handle_ruleset_search_result(const RuleSets::Search::Result& result, ConstContextRef context) {
  if (result) {
    log_.infof(component, "successfully queried rulesets");
    print_rulesets(context.get().cout, result.value());
    context_->stop();
  } else {
    log_.errorf(component, "failed to query for rulesets: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
