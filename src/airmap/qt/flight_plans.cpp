#include <airmap/qt/flight_plans.h>

airmap::qt::FlightPlans::FlightPlans(const std::shared_ptr<Dispatcher>& dispatcher,
                                     const std::shared_ptr<Client>& client)
    : dispatcher_{dispatcher}, client_{client} {
}

void airmap::qt::FlightPlans::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->flight_plans().for_id(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::FlightPlans::create_by_polygon(const Create::Parameters& parameters, const Create::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->flight_plans().create_by_polygon(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::FlightPlans::update(const Update::Parameters& parameters, const Update::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->flight_plans().update(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::FlightPlans::delete_(const Delete::Parameters& parameters, const Delete::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->flight_plans().delete_(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::FlightPlans::render_briefing(const RenderBriefing::Parameters& parameters,
                                              const RenderBriefing::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->flight_plans().render_briefing(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::FlightPlans::submit(const Submit::Parameters& parameters, const Submit::Callback& cb) {
  dispatcher_->dispatch_to_native([ this, sp = shared_from_this(), parameters, cb ]() {
    sp->client_->flight_plans().submit(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}
