#include <airmap/rest/authenticator.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <fmt/printf.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

template <typename... Args>
std::string version_to_path(airmap::Client::Version version, const char* pattern, Args&&... args) {
  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1", std::forward<Args>(args)...);
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage", std::forward<Args>(args)...);
  }

  throw std::logic_error{"should not reach here"};
}

}  // namespace

airmap::rest::Authenticator::Authenticator(Client::Version version,
                                           const std::shared_ptr<net::http::Requester>& airmap_requester,
                                           const std::shared_ptr<net::http::Requester>& sso_requester)
    : version_{version}, airmap_requester_{airmap_requester}, sso_requester_{sso_requester} {
}

void airmap::rest::Authenticator::authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                                             const AuthenticateWithPassword::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  sso_requester_->post("/oauth/ro", std::move(headers), j.dump(), [cb](const net::http::Requester::Result& result) {
    if (result) {
      cb(jsend::to_outcome<OAuthToken>(json::parse(result.value().body)));
    } else {
      cb(AuthenticateWithPassword::Result{result.error()});
    }
  });
}

void airmap::rest::Authenticator::authenticate_anonymously(const AuthenticateAnonymously::Params& params,
                                                           const AuthenticateAnonymously::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  airmap_requester_->post(version_to_path(version_, "/auth/%s/anonymous/token"), std::move(headers), j.dump(),
                          [cb](const net::http::Requester::Result& result) {
                            if (result) {
                              cb(jsend::to_outcome<AnonymousToken>(json::parse(result.value().body)));
                            } else {
                              cb(AuthenticateAnonymously::Result{result.error()});
                            }
                          });
}

void airmap::rest::Authenticator::renew_authentication(const RenewAuthentication::Params& params,
                                                       const RenewAuthentication::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  sso_requester_->post("/delegation", std::move(headers), j.dump(), [cb](const net::http::Requester::Result& result) {
    if (result) {
      cb(jsend::to_outcome<RefreshedToken>(json::parse(result.value().body)));
    } else {
      cb(RenewAuthentication::Result{result.error()});
    }
  });
}
