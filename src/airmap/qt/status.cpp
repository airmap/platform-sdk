#include <airmap/qt/status.h>

airmap::qt::Status::Status(const std::shared_ptr<Dispatcher>& dispatcher, const std::shared_ptr<airmap::Client>& client)
    : dispatcher_{dispatcher}, client_{client} {
}

void airmap::qt::Status::get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->status().get_status_by_point(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::Status::get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->status().get_status_by_path(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::Status::get_status_by_polygon(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->status().get_status_by_polygon(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}
