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
//  aircrafts.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/rest/aircrafts.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Aircrafts::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/aircraft/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v2");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Aircrafts::Aircrafts(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Aircrafts::manufacturers(const Manufacturers::Parameters& parameters,
                                            const Manufacturers::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/manufacturer", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<std::vector<Aircraft::Manufacturer>>(cb));
}

void airmap::rest::Aircrafts::models(const Models::Parameters& parameters, const Models::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/model", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<std::vector<Aircraft>>(cb));
}

void airmap::rest::Aircrafts::model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  requester_->get(fmt::sprintf("/model/%s", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Aircraft>(cb));
}
