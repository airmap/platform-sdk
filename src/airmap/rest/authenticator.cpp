//
//  authenticator.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/rest/authenticator.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Authenticator::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/auth/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Authenticator::Authenticator(const std::shared_ptr<net::http::Requester>& airmap_requester,
                                           const std::shared_ptr<net::http::Requester>& sso_requester)
    : airmap_requester_{airmap_requester}, sso_requester_{sso_requester} {
}

void airmap::rest::Authenticator::authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                                             const AuthenticateWithPassword::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  sso_requester_->post("/oauth/ro", std::move(headers), j.dump(), [cb](const auto& result) {
    if (result) {
      const auto& response = result.value();

      switch (response.classify()) {
        case net::http::Response::Classification::informational:
        case net::http::Response::Classification::redirection:
          break;
        case net::http::Response::Classification::success:
        case net::http::Response::Classification::server_error:
          cb(jsend::parse_to_outcome<Token::OAuth>(result.value().body));
          break;
        case net::http::Response::Classification::client_error:
          try {
            auto j = nlohmann::json::parse(result.value().body);
            if (j.count("error_description") > 0) {
              cb(Outcome<Token::OAuth, Error>{Error{j["error_description"].template get<std::string>()}});
            } else {
              cb(Outcome<Token::OAuth, Error>{Error{"generic client error"}});
            }
          } catch (...) {
            cb(Outcome<Token::OAuth, Error>{Error{"generic client error"}});
          }
          break;
        default:
          cb(Outcome<Token::OAuth, Error>{
              Error{"networking error"}
                  .value(Error::Value{std::string{"status-code"}},
                         Error::Value{static_cast<std::int64_t>(result.value().status)})
                  .value(Error::Value{std::string{"body"}}, Error::Value{result.value().body})});
          break;
      }
    } else {
      cb(Outcome<Token::OAuth, Error>{result.error()});
    }
  });
}

void airmap::rest::Authenticator::authenticate_anonymously(const AuthenticateAnonymously::Params& params,
                                                           const AuthenticateAnonymously::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  airmap_requester_->post("/anonymous/token", std::move(headers), j.dump(),
                          net::http::jsend_parsing_request_callback<Token::Anonymous>(cb));
}

void airmap::rest::Authenticator::renew_authentication(const RenewAuthentication::Params& params,
                                                       const RenewAuthentication::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  sso_requester_->post("/delegation", std::move(headers), j.dump(),
                       net::http::jsend_parsing_request_callback<Token::Refreshed>(cb));
}
