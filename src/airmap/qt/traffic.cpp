#include <airmap/qt/traffic.h>

airmap::qt::Traffic::Monitor::Monitor(const std::shared_ptr<Dispatcher>& dispatcher,
                                      const std::shared_ptr<airmap::Traffic::Monitor>& native)
    : dispatcher_{dispatcher}, native_{native} {
}

void airmap::qt::Traffic::Monitor::subscribe(const std::shared_ptr<airmap::Traffic::Monitor::Subscriber>& subscriber) {
  dispatcher_->dispatch_to_qt([ this, sp = shared_from_this(), subscriber ] { sp->subscribers_.insert(subscriber); });
}

void airmap::qt::Traffic::Monitor::unsubscribe(
    const std::shared_ptr<airmap::Traffic::Monitor::Subscriber>& subscriber) {
  dispatcher_->dispatch_to_qt([ this, sp = shared_from_this(), subscriber ] { sp->subscribers_.erase(subscriber); });
}

void airmap::qt::Traffic::Monitor::handle_update(Update::Type type, const std::vector<Update>& update) {
  dispatcher_->dispatch_to_qt([ this, sp = shared_from_this(), type, update ]() {
    for (const auto& subscriber : sp->subscribers_)
      subscriber->handle_update(type, update);
  });
}

airmap::qt::Traffic::Traffic(const std::shared_ptr<Dispatcher>& dispatcher,
                             const std::shared_ptr<airmap::Client>& client)
    : dispatcher_{dispatcher}, client_{client} {
}

void airmap::qt::Traffic::monitor(const Monitor::Params& parameters, const Monitor::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->traffic().monitor(parameters, [this, sp, parameters, cb](const auto& result) {
      if (result) {
        auto m  = result.value();
        auto mm = std::make_shared<Monitor>(sp->dispatcher_, m);
        m->subscribe(mm);
        sp->dispatcher_->dispatch_to_qt([sp, mm, cb]() { cb(Monitor::Result{mm}); });
      } else {
        sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
      }
    });
  });
}
