#include <airmap/rest/authenticator.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

airmap::rest::Authenticator::Authenticator(Communicator& communicator) : communicator_{communicator} {
}

void airmap::rest::Authenticator::authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                                             const AuthenticateWithPassword::Callback& cb) {
  //throw std::runtime_error{"not implemented"};
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  communicator_.post("https://sso.airmap.io", "/oauth/ro", std::move(headers), j.dump(),
                     [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<OAuthToken>(json::parse(result.value())));
                       }
                     });
}

void airmap::rest::Authenticator::authenticate_anonymously(const AuthenticateAnonymously::Params& params,
                                                           const AuthenticateAnonymously::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  communicator_.post("https://api.airmap.com", "/auth/v1/anonymous/token", std::move(headers), j.dump(),
                     [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<AnonymousToken>(json::parse(result.value())));
                       }
                     });
}

void airmap::rest::Authenticator::renew_authentication(const RenewAuthentication::Params&,
                                                       const RenewAuthentication::Callback&) {
}