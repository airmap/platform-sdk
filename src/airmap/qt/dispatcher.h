#ifndef AIRMAP_QT_DISPATCHER_H_
#define AIRMAP_QT_DISPATCHER_H_

#include <airmap/context.h>

#include <QEvent>
#include <QObject>

#include <functional>

namespace airmap {
namespace qt {

class Dispatcher : public QObject {
 public:
  class Event : public QEvent {
   public:
    static Type registered_type();

    explicit Event(const std::function<void()>& task);
    void dispatch();

   private:
    std::function<void()> task_;
  };

  using Task = std::function<void()>;

  explicit Dispatcher(const std::shared_ptr<Context>& context);

  void dispatch_to_qt(const Task& task);
  void dispatch_to_native(const Task& task);

  // From QObject
  bool event(QEvent* event) override;

 private:
  std::shared_ptr<Context> context_;
};

}  // namespace qt
}  // namespace airmap

#endif  // AIRMAP_QT_DISPATCHER_H_
