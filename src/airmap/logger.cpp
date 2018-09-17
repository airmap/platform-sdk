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
#include <airmap/logger.h>

#include <airmap/date_time.h>

#include <boost/asio.hpp>

#include <boost/core/null_deleter.hpp>
#include <boost/log/attributes/attribute_name.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>

#include <nlohmann/json.hpp>

#include <unistd.h>

#include <map>

namespace ip      = boost::asio::ip;
namespace logging = boost::log;
namespace sinks   = logging::sinks;
namespace srcs    = logging::sources;

using json = nlohmann::json;

namespace {
namespace key {

static const logging::attribute_name severity{"severity"};
static const logging::attribute_name component{"component"};
static const logging::attribute_name message{"message"};

}  // namespace key

struct NullLogger : public airmap::Logger {
  NullLogger() = default;
  void log(Severity, const char*, const char*) override {
  }
  bool should_log(Severity, const char*, const char*) override {
    return false;
  }
};

class BunyanFormatter {
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

  void format(const logging::record_view& rec, logging::formatting_ostream& strm) const {
    json j;

    j["v"] = bunyan_version();
    j["level"] =
        severity_lut_.at(rec.attribute_values()[key::severity].extract<logging::trivial::severity_level>().get());
    j["name"]     = rec.attribute_values()[key::component].extract<std::string>().get();
    j["hostname"] = hostname_;
    j["pid"]      = pid_;
    j["time"]     = airmap::iso8601::generate(airmap::Clock::local_time());
    j["msg"]      = rec.attribute_values()[key::message].extract<std::string>().get();

    strm << j.dump();
    strm.flush();
  }

 private:
  std::map<logging::trivial::severity_level, uint> severity_lut_{
      {logging::trivial::severity_level::trace, 10}, {logging::trivial::severity_level::debug, 20},
      {logging::trivial::severity_level::info, 30},  {logging::trivial::severity_level::warning, 40},
      {logging::trivial::severity_level::error, 50}, {logging::trivial::severity_level::fatal, 60},
  };

  std::string hostname_;
  pid_t pid_;
};

class DefaultLogger : public airmap::Logger {
 public:
  explicit DefaultLogger(std::ostream& out) : sink_{new Sink{true}} {
    sink_->locked_backend()->add_stream(boost::shared_ptr<std::ostream>{&out, boost::null_deleter()});

    sink_->set_formatter(
        [this](const logging::record_view& rec, logging::formatting_ostream& strm) { formatter_.format(rec, strm); });

    logging::core::get()->add_sink(sink_);
  }

  // From airmap::Logger
  void log(Severity severity, const char* message, const char* component) override {
    auto record = logger_.open_record();

    switch (severity) {
      case Severity::debug:
        record.attribute_values().insert(
            key::severity, logging::attributes::make_attribute_value(logging::trivial::severity_level::debug));
        break;
      case Severity::info:
        record.attribute_values().insert(
            key::severity, logging::attributes::make_attribute_value(logging::trivial::severity_level::info));
        break;
      case Severity::error:
        record.attribute_values().insert(
            key::severity, logging::attributes::make_attribute_value(logging::trivial::severity_level::error));
        break;
    }

    if (component) {
      record.attribute_values().insert(key::component,
                                       logging::attributes::make_attribute_value(std::string{component}));
    }

    if (message) {
      record.attribute_values().insert(key::message, logging::attributes::make_attribute_value(std::string{message}));
    }

    logger_.push_record(std::move(record));
  }

  bool should_log(Severity, const char*, const char*) override {
    return true;
  }

 private:
  using Sink = sinks::asynchronous_sink<sinks::text_ostream_backend>;

  BunyanFormatter formatter_;
  boost::shared_ptr<Sink> sink_;
  srcs::logger_mt logger_;
};

class FilteringLogger : public airmap::Logger {
 public:
  explicit FilteringLogger(Severity severity, const std::shared_ptr<Logger>& next) : severity_{severity}, next_{next} {
  }

  // From airmap::Logger
  void log(Severity severity, const char* message, const char* component) override {
    if (should_log(severity, message, component))
      next_->log(severity, message, component);
  }

  bool should_log(Severity severity, const char*, const char*) override {
    return severity >= severity_;
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

std::istream& airmap::operator>>(std::istream& in, Logger::Severity& severity) {
  std::string s;
  in >> s;

  if (s == "debug") {
    severity = Logger::Severity::debug;
  } else if (s == "info") {
    severity = Logger::Severity::info;
  } else if (s == "error") {
    severity = Logger::Severity::error;
  }

  return in;
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
