//
//  airspaces.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/rest/airspaces.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <sstream>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Airspaces::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/airspace/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v2");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Airspaces::Airspaces(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Airspaces::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/search", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<std::vector<Airspace>>(cb));
}

void airmap::rest::Airspaces::for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  requester_->get(fmt::sprintf("/%s", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Airspace>(cb));
}
