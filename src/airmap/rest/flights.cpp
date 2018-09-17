// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <airmap/rest/flights.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Flights::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/flight/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v2");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Flights::Flights(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Flights::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  requester_->get("/", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Search::Response>(cb));
}

void airmap::rest::Flights::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  requester_->get(fmt::sprintf("/%s", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Flight>(cb));
}

void airmap::rest::Flights::create_flight_by_point(const CreateFlight::Parameters& parameters,
                                                   const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  requester_->post("/point", std::move(headers), j.dump(), net::http::jsend_parsing_request_callback<Flight>(cb));
}

void airmap::rest::Flights::create_flight_by_path(const CreateFlight::Parameters& parameters,
                                                  const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  requester_->post("/path", std::move(headers), j.dump(), net::http::jsend_parsing_request_callback<Flight>(cb));
}

void airmap::rest::Flights::create_flight_by_polygon(const CreateFlight::Parameters& parameters,
                                                     const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  requester_->post("/polygon", std::move(headers), j.dump(), net::http::jsend_parsing_request_callback<Flight>(cb));
}

void airmap::rest::Flights::end_flight(const EndFlight::Parameters& parameters, const EndFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  requester_->post(fmt::sprintf("/%s/end", parameters.id), std::move(headers), std::string{},
                   net::http::jsend_parsing_request_callback<EndFlight::Response>(cb));
}

void airmap::rest::Flights::delete_flight(const DeleteFlight::Parameters& parameters,
                                          const DeleteFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  requester_->post(fmt::sprintf("/%s/delete", parameters.id), std::move(headers), std::string{},
                   net::http::jsend_parsing_request_callback<DeleteFlight::Response>(cb));
}

void airmap::rest::Flights::start_flight_communications(const StartFlightCommunications::Parameters& parameters,
                                                        const StartFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  requester_->post(fmt::sprintf("/%s/start-comm", parameters.id), std::move(headers), std::string{},
                   net::http::jsend_parsing_request_callback<StartFlightCommunications::Response>(cb));
}

void airmap::rest::Flights::end_flight_communications(const EndFlightCommunications::Parameters& parameters,
                                                      const EndFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  requester_->post(fmt::sprintf("/%s/end-comm", parameters.id), std::move(headers), std::string{},
                   net::http::jsend_parsing_request_callback<EndFlightCommunications::Response>(cb));
}
