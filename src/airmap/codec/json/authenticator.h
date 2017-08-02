
#ifndef AIRMAP_CODEC_JSON_AUTHENTICATOR_H_
#define AIRMAP_CODEC_JSON_AUTHENTICATOR_H_

#include <airmap/authenticator.h>

#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Authenticator::OAuthToken& token) {
  get(token.refresh, j, "refresh_token");
  get(token.id, j, "id_token");
  get(token.access, j, "access_token");
  get(token.type, j, "token_type");
}

inline void decode(const nlohmann::json& j, Authenticator::OAuthToken::Type& type) {
  auto t = j.get<std::string>();
  if (t == "bearer")
    type = Authenticator::OAuthToken::Type::bearer;
}

inline void decode(const nlohmann::json& j, Authenticator::AnonymousToken& token) {
  get(token.id, j, "id_token");
}

inline void decode(const nlohmann::json& j, Authenticator::RefreshedToken& token) {
  get(token.type, j, "token_type");
  get(token.expires_in, j, "expires_in");
  get(token.id, j, "id_token");
}

inline void decode(const nlohmann::json& j, Authenticator::RefreshedToken::Type& type) {
  auto t = j.get<std::string>();
  if (t == "Bearer")
    type = Authenticator::RefreshedToken::Type::bearer;
}

inline void encode(nlohmann::json& j, const Authenticator::AuthenticateWithPassword::Params& params) {
  j["client_id"]  = params.client_id;
  j["connection"] = params.connection_name;
  j["username"]   = params.username;
  j["password"]   = params.password;
  j["device"]     = params.device;

  switch (params.grant_type) {
    case Authenticator::GrantType::BEARER:
      j["grant_type"] = "urn:ietf:params:oauth:grant-type:jwt-bearer";
      break;
    case Authenticator::GrantType::PASSWORD:
      j["grant_type"] = "password";
    default:
      break;
  }

  switch (params.scope) {
    case Authenticator::Scope::OPEN_ID:
      j["scope"] = "openid";
      break;
    case Authenticator::Scope::OPEN_ID_OFFLINE_ACCESS:
      j["scope"] = "openid offline access";
      break;
    case Authenticator::Scope::ACCESS_TOKEN:
      j["scope"] = "";
    default:
      break;
  }
}

inline void encode(nlohmann::json& j, const Authenticator::AuthenticateAnonymously::Params& params) {
  j["user_id"] = params.user_id;
}

inline void encode(nlohmann::json& j, const Authenticator::RenewAuthentication::Params& params) {
  j["client_id"] = params.client_id;
  j["device"]    = params.device;
  j["id_token"]  = params.id_token;

  switch (params.grant_type) {
    case Authenticator::GrantType::BEARER:
      j["grant_type"] = "urn:ietf:params:oauth:grant-type:jwt-bearer";
      break;
    case Authenticator::GrantType::PASSWORD:
      j["grant_type"] = "password";
    default:
      break;
  }

  switch (params.scope) {
    case Authenticator::Scope::OPEN_ID:
      j["scope"] = "openid";
      break;
    case Authenticator::Scope::OPEN_ID_OFFLINE_ACCESS:
      j["scope"] = "openid offline access";
      break;
    case Authenticator::Scope::ACCESS_TOKEN:
      j["scope"] = "";
    default:
      break;
  }
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AUTHENTICATOR_H_