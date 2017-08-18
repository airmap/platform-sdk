#include <airmap/logger.h>

#include <airmap/date_time.h>

#include <fmt/format.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/ostream_sink.h>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <unistd.h>

#include <unordered_map>

namespace ip = boost::asio::ip;
using json   = nlohmann::json;

namespace {

struct NullLogger : public airmap::Logger {
  NullLogger() = default;
  void log(Severity, const char*, const char*) override {
  }
};

class BunyanFormatter : public spdlog::formatter {
 public:
  static uint bunyan_version() {
    return 0;
  }

  BunyanFormatter() : pid_{::getpid()} {
    boost::system::error_code ec;
    hostname_ = ip::host_name(ec);
    if (ec)
      hostname_ = "unknown";
  }

  void format(spdlog::details::log_msg& msg) override {
    json j;

    j["v"]        = bunyan_version();
    j["level"]    = severity_lut_.at(msg.level);
    j["name"]     = msg.logger_name ? *msg.logger_name : "undefined";
    j["hostname"] = hostname_;
    j["pid"]      = pid_;
    j["time"]     = airmap::iso8601::generate(airmap::Clock::local_time());
    j["msg"]      = msg.raw.str();

    msg.formatted << j.dump() << '\n';
  }

 private:
  std::unordered_map<spdlog::level::level_enum, uint> severity_lut_{
      {spdlog::level::trace, 10}, {spdlog::level::debug, 20}, {spdlog::level::info, 30},
      {spdlog::level::warn, 40},  {spdlog::level::err, 50},   {spdlog::level::critical, 60},
  };
  std::string hostname_;
  pid_t pid_;
};

class DefaultLogger : public airmap::Logger {
 public:
  explicit DefaultLogger(std::ostream& out) : logger_{"airmap", std::make_shared<spdlog::sinks::ostream_sink_mt>(out)} {
    logger_.set_formatter(std::make_shared<BunyanFormatter>());
    logger_.set_level(spdlog::level::debug);
  }

  // From airmap::Logger
  void log(Severity severity, const char* message, const char*) override {
    switch (severity) {
      case Severity::debug:
        logger_.log(spdlog::level::debug, message);
        break;
      case Severity::info:
        logger_.log(spdlog::level::info, message);
        break;
      case Severity::error:
        logger_.log(spdlog::level::err, message);
        break;
    }
    logger_.flush();
  }

 private:
  spdlog::logger logger_;
};

class FilteringLogger : public airmap::Logger {
 public:
  explicit FilteringLogger(Severity severity, const std::shared_ptr<Logger>& next) : severity_{severity}, next_{next} {
  }

  // From airmap::Logger
  void log(Severity severity, const char* message, const char* component) override {
    if (severity < severity_)
      return;
    next_->log(severity, message, component);
  }

 private:
  Severity severity_;
  std::shared_ptr<Logger> next_;
};

}  // namespace

void airmap::Logger::debug(const char* message, const char* component) {
  log(Severity::debug, message, component);
}

void airmap::Logger::info(const char* message, const char* component) {
  log(Severity::info, message, component);
}

void airmap::Logger::error(const char* message, const char* component) {
  log(Severity::error, message, component);
}

bool airmap::operator<(Logger::Severity lhs, Logger::Severity rhs) {
  using UT = typename std::underlying_type<Logger::Severity>::type;
  return static_cast<UT>(lhs) < static_cast<UT>(rhs);
}

std::shared_ptr<airmap::Logger> airmap::create_default_logger(std::ostream& out) {
  return std::make_shared<DefaultLogger>(out);
}

std::shared_ptr<airmap::Logger> airmap::create_filtering_logger(Logger::Severity severity,
                                                                const std::shared_ptr<Logger>& logger) {
  return std::make_shared<FilteringLogger>(severity, logger);
}

std::shared_ptr<airmap::Logger> airmap::create_null_logger() {
  return std::make_shared<NullLogger>();
}