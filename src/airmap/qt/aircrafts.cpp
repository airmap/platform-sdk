#include <airmap/qt/aircrafts.h>

airmap::qt::Aircrafts::Aircrafts(const std::shared_ptr<Dispatcher>& dispatcher, const std::shared_ptr<Client>& client)
    : dispatcher_{dispatcher}, client_{client} {
}

void airmap::qt::Aircrafts::manufacturers(const Manufacturers::Parameters& parameters,
                                          const Manufacturers::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->aircrafts().manufacturers(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::Aircrafts::models(const Models::Parameters& parameters, const Models::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->aircrafts().models(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::Aircrafts::model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->aircrafts().model_for_id(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}
