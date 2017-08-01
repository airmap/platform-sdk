
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

inline void encode(nlohmann::json& j, const Authenticator::AuthenticateWithPassword::Params& params) {
  j["client_id"]  = params.client_id;
  j["connection"] = params.connection_name;
  j["username"]   = params.username;
  j["password"]   = params.password;
  j["device"]     = params.device;
  j["grant_type"] = params.grant_type;
  j["scope"]      = params.scope;
}

inline void encode(nlohmann::json& j, const Authenticator::AuthenticateAnonymously::Params& params) {
  j["user_id"] = params.user_id;
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AUTHENTICATOR_H_