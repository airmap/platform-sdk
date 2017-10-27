#include <airmap/qt/dispatcher.h>

#include <QCoreApplication>

QEvent::Type airmap::qt::Dispatcher::Event::registered_type() {
  static const Type rt = static_cast<Type>(registerEventType());
  return rt;
}

airmap::qt::Dispatcher::Event::Event(const std::function<void()>& task) : QEvent{registered_type()}, task_{task} {
}

void airmap::qt::Dispatcher::Event::dispatch() {
  task_();
}

airmap::qt::Dispatcher::Dispatcher(const std::shared_ptr<Context>& context) : context_{context} {
}

void airmap::qt::Dispatcher::dispatch_to_qt(const std::function<void()>& task) {
  QCoreApplication::postEvent(this, new Event{task});
}

void airmap::qt::Dispatcher::dispatch_to_native(const std::function<void()>& task) {
  context_->dispatch(task);
}

// From QObject
bool airmap::qt::Dispatcher::event(QEvent* event) {
  if (event->type() == Event::registered_type()) {
    event->accept();

    if (auto e = dynamic_cast<Event*>(event)) {
      e->dispatch();
    }

    return true;
  }

  return false;
}
