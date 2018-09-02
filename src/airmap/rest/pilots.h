//
//  pilots.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_REST_PILOTS_H_
#define AIRMAP_REST_PILOTS_H_

#include <airmap/pilots.h>

#include <airmap/client.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

class Pilots : public airmap::Pilots {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit Pilots(const std::shared_ptr<net::http::Requester>& requester);

  void authenticated(const Authenticated::Parameters& parameters, const Authenticated::Callback& cb) override;
  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;
  void update_for_id(const UpdateForId::Parameters& parameters, const UpdateForId::Callback& cb) override;
  void start_verify_pilot_phone_for_id(const StartVerifyPilotPhoneForId::Parameters& parameters,
                                       const StartVerifyPilotPhoneForId::Callback& cb) override;
  void finish_verify_pilot_phone_for_id(const FinishVerifyPilotPhoneForId::Parameters& parameters,
                                        const FinishVerifyPilotPhoneForId::Callback& cb) override;
  void aircrafts(const Aircrafts::Parameters& parameters, const Aircrafts::Callback& cb) override;
  void add_aircraft(const AddAircraft::Parameters& parameters, const AddAircraft::Callback& cb) override;
  void delete_aircraft(const DeleteAircraft::Parameters& parameters, const DeleteAircraft::Callback& cb) override;
  void update_aircraft(const UpdateAircraft::Parameters& parameters, const UpdateAircraft::Callback& cb) override;

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_PILOTS_H_
