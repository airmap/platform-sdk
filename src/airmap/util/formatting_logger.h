/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  formatting_logger.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_UTIL_FORMATTING_LOGGER_H_
#define AIRMAP_UTIL_FORMATTING_LOGGER_H_

#include <airmap/logger.h>
#include <airmap/util/fmt.h>

#include <sstream>

namespace airmap {
namespace util {

class FormattingLogger {
 public:
  template <Logger::Severity severity>
  class Record {
   public:
    explicit Record(const char* component, const std::shared_ptr<Logger>& logger)
        : component_{component}, logger_{logger} {
    }

    Record(Record&& other)
        : component_{std::move(other.component_)}, logger_{std::move(other.logger_)}, oss_{std::move(other.oss_)} {
    }

    ~Record() {
      logger_->log(severity, oss_.str().c_str(), component_);
    }

    template <typename... Args>
    inline Record& printf(const char* format, Args... args) {
      oss_ << fmt::sprintf(format, std::forward<Args>(args)...);
      return *this;
    }

    template <typename T>
    inline Record& print(const T& value) {
      oss_ << value;
      return *this;
    }

   private:
    const char* component_;
    std::shared_ptr<Logger> logger_;
    std::stringstream oss_;
  };

  explicit FormattingLogger(const std::shared_ptr<Logger>& logger) : logger_{logger} {
  }

  Record<Logger::Severity::debug> debug(const char* component) {
    return Record<Logger::Severity::debug>{component, logger_};
  }

  template <typename... Args>
  void debugf(const char* component, const char* format, Args... args) {
    if (logger_->should_log(Logger::Severity::debug, nullptr, component))
      logger_->debug(fmt::sprintf(format, std::forward<Args>(args)...).c_str(), component);
  }

  Record<Logger::Severity::info> info(const char* component) {
    return Record<Logger::Severity::info>{component, logger_};
  }

  template <typename... Args>
  void infof(const char* component, const char* format, Args... args) {
    if (logger_->should_log(Logger::Severity::info, nullptr, component))
      logger_->info(fmt::sprintf(format, std::forward<Args>(args)...).c_str(), component);
  }

  Record<Logger::Severity::error> error(const char* component) {
    return Record<Logger::Severity::error>{component, logger_};
  }

  template <typename... Args>
  void errorf(const char* component, const char* format, Args... args) {
    if (logger_->should_log(Logger::Severity::error, nullptr, component))
      logger_->error(fmt::sprintf(format, std::forward<Args>(args)...).c_str(), component);
  }

  const std::shared_ptr<Logger>& logger() const {
    return logger_;
  }

 private:
  std::shared_ptr<Logger> logger_;
};

template <Logger::Severity severity, typename T>
inline FormattingLogger::Record<severity>& operator<<(FormattingLogger::Record<severity>& record, const T& value) {
  return record.print(value);
}

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_FORMATTING_LOGGER_H_
