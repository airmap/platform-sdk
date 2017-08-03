#include <airmap/logger.h>

#include <airmap/date_time.h>

#include <boost/asio.hpp>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/ostream_sink.h>

#include <unistd.h>

#include <unordered_map>

namespace ip = boost::asio::ip;
using json = nlohmann::json;

namespace {

class BunyanFormatter : public spdlog::formatter {
 public:
  static uint bunyan_version() { return 0; }

  BunyanFormatter() : pid_{::getpid()} {
    boost::system::error_code ec;
    hostname_ = ip::host_name(ec);
    if (ec)
      hostname_ = "unknown";
  }

  void format(spdlog::details::log_msg& msg) override {
    json j;

    j["v"] = bunyan_version();
    j["level"] = severity_lut_.at(msg.level);
    j["name"] = msg.logger_name ? *msg.logger_name : "undefined";
    j["hostname"] = hostname_;
    j["pid"] = pid_;
    j["time"] = airmap::iso8601::generate(airmap::Clock::local_time());
    j["msg"] = msg.raw.str();

    msg.formatted << j.dump() << '\n';
  }

 private:
  std::unordered_map<spdlog::level::level_enum, const char*> severity_lut_{
      {spdlog::level::trace, "trace"}, {spdlog::level::debug, "debug"}, {spdlog::level::info, "info"},
      {spdlog::level::warn, "warn"},   {spdlog::level::err, "error"},   {spdlog::level::critical, "critical"},
  };
  std::string hostname_;
  pid_t pid_;
};

class DefaultLogger : public airmap::Logger {
 public:
  constexpr static std::size_t buffer_size_in_bytes{(1 << 9) * 1024};

  explicit DefaultLogger(std::ostream& out) : logger_{"airmap", std::make_shared<spdlog::sinks::ostream_sink_mt>(out)} {
    logger_.set_formatter(std::make_shared<BunyanFormatter>());
  }

  // From airmap::Logger
  void log(Severity severity, const char* message, const char*) override {
    switch (severity) {
      case Severity::info:
        logger_.log(spdlog::level::info, message);
        break;
      case Severity::error:
        logger_.log(spdlog::level::err, message);
        break;
    }
  }

 private:
  spdlog::logger logger_;
};

}  // namespace

void airmap::Logger::info(const char* message, const char* component) {
  log(Severity::info, message, component);
}

void airmap::Logger::error(const char* message, const char* component) {
  log(Severity::error, message, component);
}

std::shared_ptr<airmap::Logger> airmap::create_default_logger(std::ostream& out) {
  return std::make_shared<DefaultLogger>(out);
}