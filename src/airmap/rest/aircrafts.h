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
//  aircrafts.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_REST_AIRCRAFTS_H_
#define AIRMAP_REST_AIRCRAFTS_H_

#include <airmap/aircrafts.h>
#include <airmap/client.h>
#include <airmap/net/http/requester.h>

#include <string>

namespace airmap {
namespace rest {

class Aircrafts : public airmap::Aircrafts {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit Aircrafts(const std::shared_ptr<net::http::Requester>& requester);

  void manufacturers(const Manufacturers::Parameters& parameters, const Manufacturers::Callback& cb) override;
  void models(const Models::Parameters& parameters, const Models::Callback& cb) override;
  void model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) override;

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AIRCRAFTS_H_
