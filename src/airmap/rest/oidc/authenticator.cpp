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
#include <airmap/rest/oidc/authenticator.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::oidc::Authenticator::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/auth/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::oidc::Authenticator::Authenticator(const std::shared_ptr<net::http::Requester>& requester)
    : requester_{requester} {
}

void airmap::rest::oidc::Authenticator::authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                                             const AuthenticateWithPassword::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  std::unordered_map<std::string, std::string> body;
  body["grant_type"] = "password";
  body["username"] = params.oauth.username;
  body["password"] = params.oauth.password;
  body["client_id"] = params.oauth.client_id;
  body["device_id"] = params.oauth.device_id;
  body["scopes"] = "openid";

  requester_->post_form("/realms/airmap/protocol/openid-connect/token", std::move(headers), std::move(body), [cb](const auto& result) {
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

void airmap::rest::oidc::Authenticator::authenticate_anonymously(const AuthenticateAnonymously::Params& params,
                                                           const AuthenticateAnonymously::Callback& cb) {
  throw std::runtime_error{"not implemented"};
}

void airmap::rest::oidc::Authenticator::renew_authentication(const RenewAuthentication::Params& params,
                                                       const RenewAuthentication::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  std::unordered_map<std::string, std::string> body;
  body["grant_type"] = "refresh_token";
  body["client_id"] = params.client_id;
  body["refresh_token"] = params.refresh_token;
  
  requester_->post_form("/delegation", std::move(headers), std::move(body),
                       net::http::jsend_parsing_request_callback<Token::Refreshed>(cb));
}
