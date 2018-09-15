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
