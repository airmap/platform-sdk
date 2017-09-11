#include <airmap/codec/json/authenticator.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::encode(nlohmann::json& j, const Authenticator::GrantType& type) {
  switch (type) {
    case Authenticator::GrantType::bearer:
      j["grant_type"] = "urn:ietf:params:oauth:grant-type:jwt-bearer";
      break;
    case Authenticator::GrantType::password:
      j["grant_type"] = "password";
    default:
      break;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const Authenticator::Scope& scope) {
  switch (scope) {
    case Authenticator::Scope::open_id:
      j["scope"] = "openid";
      break;
    case Authenticator::Scope::open_id_offline_access:
      j["scope"] = "openid offline_access";
      break;
    case Authenticator::Scope::access_token:
      j["scope"] = "";
    default:
      break;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const Authenticator::Connection& connection) {
  switch (connection) {
    case Authenticator::Connection::username_password_authentication:
      j["connection"] = "Username-Password-Authentication";
      break;
    default:
      break;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const Authenticator::AuthenticateWithPassword::Params& params) {
  j["username"]  = params.oauth.username;
  j["password"]  = params.oauth.password;
  j["client_id"] = params.oauth.client_id;
  j["device"]    = params.oauth.device_id;

  encode(j, params.grant_type);
  encode(j, params.scope);
  encode(j, params.connection);
}

void airmap::codec::json::encode(nlohmann::json& j, const Authenticator::AuthenticateAnonymously::Params& params) {
  j["user_id"] = params.id;
}

void airmap::codec::json::encode(nlohmann::json& j, const Authenticator::RenewAuthentication::Params& params) {
  j["client_id"]     = params.client_id;
  j["refresh_token"] = params.refresh_token;

  encode(j, params.grant_type);
  encode(j, params.scope);
}
