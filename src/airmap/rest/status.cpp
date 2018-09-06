//
//  status.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/rest/status.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Status::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/status/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v2");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Status::Status(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Status::get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/point", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Report>(cb));
}

void airmap::rest::Status::get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/path", std::move(query), std::move(headers), net::http::jsend_parsing_request_callback<Report>(cb));
}

void airmap::rest::Status::get_status_by_polygon(const GetStatus::Parameters& parameters,
                                                 const GetStatus::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/polygon", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Report>(cb));
}
