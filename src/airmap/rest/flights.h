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
//  flights.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_REST_FLIGHTS_H_
#define AIRMAP_REST_FLIGHTS_H_

#include <airmap/client.h>
#include <airmap/flights.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

class Flights : public airmap::Flights {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit Flights(const std::shared_ptr<net::http::Requester>& requester);

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;
  void create_flight_by_point(const CreateFlight::Parameters& parameters, const CreateFlight::Callback& cb) override;
  void create_flight_by_path(const CreateFlight::Parameters& parameters, const CreateFlight::Callback& cb) override;
  void create_flight_by_polygon(const CreateFlight::Parameters& parameters, const CreateFlight::Callback& cb) override;
  void end_flight(const EndFlight::Parameters& parameters, const EndFlight::Callback& cb) override;
  void delete_flight(const DeleteFlight::Parameters& parameters, const DeleteFlight::Callback& cb) override;
  void start_flight_communications(const StartFlightCommunications::Parameters& parameters,
                                   const StartFlightCommunications::Callback& cb) override;
  virtual void end_flight_communications(const EndFlightCommunications::Parameters& parameters,
                                         const EndFlightCommunications::Callback& cb) override;

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_FLIGHTS_H_
