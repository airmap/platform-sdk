#include <airmap/codec/json/credentials.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Credentials::Anonymous& anonymous) {
  get(anonymous.id, j, "id");
}

void airmap::codec::json::decode(const nlohmann::json& j, Credentials::OAuth& oauth) {
  get(oauth.username, j, "username");
  get(oauth.password, j, "password");
  get(oauth.client_id, j, "client-id");
  get(oauth.device_id, j, "device-id");
}

void airmap::codec::json::decode(const nlohmann::json& j, Credentials& credentials) {
  get(credentials.api_key, j, "api-key");
  get(credentials.anonymous, j, "anonymous");
  get(credentials.oauth, j, "oauth");
}
