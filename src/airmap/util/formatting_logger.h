#ifndef AIRMAP_UTIL_FORMATTING_LOGGER_H_
#define AIRMAP_UTIL_FORMATTING_LOGGER_H_

#include <airmap/logger.h>

#include <fmt/printf.h>

namespace airmap {
namespace util {

class FormattingLogger {
 public:
  explicit FormattingLogger(const std::shared_ptr<Logger>& logger) : logger_{logger} {
  }

  template <typename... Args>
  void debugf(const char* component, const char* format, Args... args) {
    logger_->debug(fmt::sprintf(format, std::forward<Args>(args)...).c_str(), component);
  }

  template <typename... Args>
  void infof(const char* component, const char* format, Args... args) {
    logger_->info(fmt::sprintf(format, std::forward<Args>(args)...).c_str(), component);
  }

  template <typename... Args>
  void errorf(const char* component, const char* format, Args... args) {
    logger_->error(fmt::sprintf(format, std::forward<Args>(args)...).c_str(), component);
  }

  const std::shared_ptr<Logger>& logger() const {
    return logger_;
  }

 private:
  std::shared_ptr<Logger> logger_;
};

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_FORMATTING_LOGGER_H_