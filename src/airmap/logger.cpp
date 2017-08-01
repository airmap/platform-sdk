#include <airmap/logger.h>

#include <airmap/date_time.h>

#include <fmt/format.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/ostream_sink.h>

#include <unistd.h>

#include <unordered_map>

namespace {

class BunyanFormatter : public spdlog::formatter {
 public:
  static constexpr const char* pattern{
      R"_({{"v":{},"level":"{}","name":"{}","hostname":"{}","pid":{},"time":"{}","msg":"{}"}}
)_"};

  static constexpr uint max_hostname_length{256};

  BunyanFormatter() : pid_{::getpid()} {
    ::gethostname(hostname_, max_hostname_length);
  }

  void format(spdlog::details::log_msg& msg) override {
    msg.formatted.write(pattern, 0, severity_lut_.at(msg.level), msg.logger_name ? *msg.logger_name : "undefined",
                        hostname_, pid_, airmap::iso8601::generate(airmap::Clock::local_time()), msg.raw.str());
  }

 private:
  std::unordered_map<spdlog::level::level_enum, const char*> severity_lut_{
      {spdlog::level::trace, "trace"}, {spdlog::level::debug, "debug"}, {spdlog::level::info, "info"},
      {spdlog::level::warn, "warn"},   {spdlog::level::err, "error"},   {spdlog::level::critical, "critical"},
  };
  char hostname_[max_hostname_length];
  pid_t pid_;
};

class DefaultLogger : public airmap::Logger {
 public:
  constexpr static std::size_t buffer_size_in_bytes{(1 << 9) * 1024};

  explicit DefaultLogger(std::ostream& out)
      : async_logger_{"airmap", std::make_shared<spdlog::sinks::ostream_sink_mt>(out), buffer_size_in_bytes} {
    async_logger_.set_formatter(std::make_shared<BunyanFormatter>());
  }

  // From airmap::Logger
  void log(Severity severity, const char* message, const char*) override {
    switch (severity) {
      case Severity::info:
        async_logger_.log(spdlog::level::info, message);
        break;
      case Severity::error:
        async_logger_.log(spdlog::level::err, message);
        break;
    }
  }

 private:
  spdlog::async_logger async_logger_;
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