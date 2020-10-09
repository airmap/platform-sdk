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
#include <airmap/rest/elevation.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Elevation::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/elevation/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Elevation::Elevation(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Elevation::get_elevation_points(const GetElevationPoints::Parameters& parameters,
                                                   const GetElevationPoints::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/ele/", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<std::vector<std::int32_t>>(cb));
}

void airmap::rest::Elevation::get_elevation_carpet(const GetElevationCarpet::Parameters& parameters,
                                                   const GetElevationCarpet::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/ele/carpet/", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Carpet>(cb));
}

void airmap::rest::Elevation::get_elevation_path(const GetElevationPath::Parameters& parameters,
                                                 const GetElevationPath::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/ele/path/", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<std::vector<Path>>(cb));
}
