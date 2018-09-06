#include <airmap/qt/logger.h>

#include <airmap/qt/dispatcher.h>

struct airmap::qt::Logger::Private {
  QLoggingCategory logging_category{"airmap"};
};

struct airmap::qt::DispatchingLogger::Private {
  std::shared_ptr<airmap::Logger> next;
  std::shared_ptr<Dispatcher::ToQt> dispatcher;
};

airmap::qt::Logger::Logger() : d_{new Private{}} {
}
airmap::qt::Logger::~Logger() {
}

QLoggingCategory& airmap::qt::Logger::logging_category() {
  static QLoggingCategory lc{"airmap"};
  return lc;
}

void airmap::qt::Logger::log(Severity severity, const char* message, const char*) {
  switch (severity) {
    case Severity::debug:
      qCDebug(logging_category(), "%s", message);
      break;
    case Severity::info:
      qCInfo(logging_category(), "%s", message);
      break;
    case Severity::error:
      qCWarning(logging_category(), "%s", message);
      break;
    default:
      break;
  }
}

bool airmap::qt::Logger::should_log(Severity severity, const char*, const char*) {
  switch (severity) {
    case Severity::debug:
      return logging_category().isDebugEnabled();
    case Severity::info:
      return logging_category().isInfoEnabled();
    case Severity::error:
      return logging_category().isWarningEnabled();
    default:
      break;
  }

  return true;
}

airmap::qt::DispatchingLogger::DispatchingLogger(const std::shared_ptr<airmap::Logger>& next)
    : d_{new Private{next, Dispatcher::ToQt::create()}} {
}

airmap::qt::DispatchingLogger::~DispatchingLogger() {
}

void airmap::qt::DispatchingLogger::log(Severity severity, const char* message, const char* component) {
  std::string cmessage{message};
  std::string ccomponent{component};
  auto cnext{d_->next};

  d_->dispatcher->dispatch([severity, cmessage, ccomponent, cnext] {
    if (cnext->should_log(severity, cmessage.c_str(), ccomponent.c_str()))
      cnext->log(severity, cmessage.c_str(), ccomponent.c_str());
  });
}

bool airmap::qt::DispatchingLogger::should_log(Severity, const char*, const char*) {
  // We have to accept all incoming log messages and postpone
  // the actual evaluation of should_log in the context of next until we
  // run on the correct thread.
  return true;
}
