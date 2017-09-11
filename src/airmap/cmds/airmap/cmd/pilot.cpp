#include <airmap/cmds/airmap/cmd/pilot.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

constexpr const char* component{"pilot"};

std::string print_aircrafts(const std::vector<airmap::Pilot::Aircraft>& v) {
  std::ostringstream ss;
  for (const auto& a : v) {
    ss << fmt::sprintf(
        "    id:           %s\n"
        "    nick:         %s\n"
        "    model:\n"
        "      id:         %s\n"
        "      name:       %s\n"
        "    manufacturer:\n"
        "      id:         %s\n"
        "      name:       %s",
        a.id, a.nick_name, a.model.model.id, a.model.model.name, a.model.manufacturer.id, a.model.manufacturer.name);
  }
  return ss.str();
}

std::string print_pilot_and_aircrafts(const airmap::Pilot& pilot, const std::vector<airmap::Pilot::Aircraft>& v) {
  return fmt::sprintf(
      "pilot:\n"
      "  id:                 %s\n"
      "  first name:         %s\n"
      "  last name:          %s\n"
      "  user name:          %s\n"
      "  picture:            %s\n"
      "  email verified:     %s\n"
      "  phone verified:     %s\n"
      "  statistics:\n"
      "    total flights:    %s\n"
      "    last flight time: %s\n"
      "    total aircrafts:  %d\n"
      "  created at:         %s\n"
      "  aircrafts:\n"
      "%s",
      pilot.id, pilot.first_name, pilot.last_name, pilot.user_name, pilot.picture_url,
      pilot.verification_status.email ? "true" : "false", pilot.verification_status.phone ? "true" : "false",
      pilot.statistics.flight.total, airmap::iso8601::generate(pilot.statistics.flight.last_flight_time),
      pilot.statistics.aircraft.total, airmap::iso8601::generate(pilot.created_at), print_aircrafts(v));
}

void print_pilot_and_aircrafts(airmap::util::FormattingLogger& log, const airmap::Pilot& pilot,
                               const std::vector<airmap::Pilot::Aircraft>& aircrafts) {
  log.infof(component, "%s", print_pilot_and_aircrafts(pilot, aircrafts));
}

}  // namespace

cmd::Pilot::Pilot()
    : cli::CommandWithFlagsAndAction{"pilot", "queries information about a pilot",
                                     "queries information about a pilot"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::api_key(api_key_));
  flag(flags::authorization(authorization_));
  flag(cli::make_flag("pilot-id", "id of pilot", pilot_id_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger(create_filtering_logger(log_level_, create_default_logger(ctxt.cout)));

    if (!api_key_) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!api_key_.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!authorization_) {
      log_.errorf(component, "missing parameter 'authorization'");
      return 1;
    }

    if (!authorization_.get().validate()) {
      log_.errorf(component, "parameter 'authorization' for accessing AirMap services must not be empty");
      return 1;
    }

    if (pilot_id_ && !pilot_id_.get().validate()) {
      log_.errorf(component, "parameter 'pilot-id' must not be empty");
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    context_            = result.value();
    auto credentials    = Credentials{};
    credentials.api_key = api_key_.get();
    auto config         = Client::default_configuration(version_, credentials);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context_->create_client_with_configuration(config, [this](const ::airmap::Context::ClientCreateResult& result) {
      if (not result) {
        try {
          std::rethrow_exception(result.error());
        } catch (const std::exception& e) {
          log_.errorf(component, "failed to create AirMap client instance: %s", e.what());
        } catch (...) {
          log_.errorf(component, "failed to create AirMap client instance");
        }
        context_->stop(::airmap::Context::ReturnCode::error);
        return;
      }

      client_ = result.value();

      if (pilot_id_) {
        Pilots::ForId::Parameters params;
        params.authorization       = authorization_.get();
        params.retrieve_statistics = true;
        client_->pilots().for_id(params, std::bind(&Pilot::handle_for_id_pilot_result, this, std::placeholders::_1));
      } else {
        Pilots::Authenticated::Parameters params;
        params.authorization       = authorization_.get();
        params.retrieve_statistics = true;
        client_->pilots().authenticated(
            params, std::bind(&Pilot::handle_authenticated_pilot_result, this, std::placeholders::_1));
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

void cmd::Pilot::handle_authenticated_pilot_result(const Pilots::Authenticated::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile for authenticated user");
    Pilots::Aircrafts::Parameters params;
    params.id            = result.value().id;
    params.authorization = authorization_.get();

    client_->pilots().aircrafts(
        params, std::bind(&Pilot::handle_aircrafts_result, this, result.value(), std::placeholders::_1));
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query information about pilot: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query information about pilot");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Pilot::handle_for_id_pilot_result(const Pilots::ForId::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile for id");
    Pilots::Aircrafts::Parameters params;
    params.id            = result.value().id;
    params.authorization = authorization_.get();

    client_->pilots().aircrafts(
        params, std::bind(&Pilot::handle_aircrafts_result, this, result.value(), std::placeholders::_1));
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query information about pilot: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query information about pilot");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void cmd::Pilot::handle_aircrafts_result(const ::airmap::Pilot& pilot, const Pilots::Aircrafts::Result& result) {
  if (result) {
    print_pilot_and_aircrafts(log_, pilot, result.value());
    context_->stop();
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query information about aircrafts: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query information about aircrafts");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}
