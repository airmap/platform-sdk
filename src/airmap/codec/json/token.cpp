//
//  token.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/token.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

#include <boost/lexical_cast.hpp>

void airmap::codec::json::decode(const nlohmann::json& j, Token& token) {
  Token::Type type = j["type"];
  switch (type) {
    case Token::Type::anonymous: {
      Token::Anonymous anonymous = j["anonymous"];
      token                      = Token(anonymous);
      break;
    }
    case Token::Type::oauth: {
      Token::OAuth oauth = j["oauth"];
      token              = Token(oauth);
      break;
    }
    case Token::Type::refreshed: {
      Token::Refreshed refreshed = j["refreshed"];
      token                      = Token(refreshed);
      break;
    }
    default:
      break;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Token::Type& type) {
  type = boost::lexical_cast<Token::Type>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, Token::OAuth& token) {
  get(token.refresh, j, "refresh_token");
  get(token.id, j, "id_token");
  get(token.access, j, "access_token");
  get(token.type, j, "token_type");
}

void airmap::codec::json::decode(const nlohmann::json& j, Token::OAuth::Type& type) {
  auto t = j.get<std::string>();
  if (t == "bearer" || t == "Bearer")
    type = Token::OAuth::Type::bearer;
}

void airmap::codec::json::decode(const nlohmann::json& j, Token::Anonymous& token) {
  get(token.id, j, "id_token");
}

void airmap::codec::json::decode(const nlohmann::json& j, Token::Refreshed& token) {
  get(token.type, j, "token_type");
  get(token.expires_in, j, "expires_in");
  get(token.id, j, "id_token");
  get(token.original_token, j, "original_token");
}

void airmap::codec::json::encode(nlohmann::json& j, const Token& token) {
  j["type"] = token.type();

  switch (token.type()) {
    case Token::Type::anonymous:
      j["anonymous"] = token.anonymous();
      break;
    case Token::Type::oauth:
      j["oauth"] = token.oauth();
      break;
    case Token::Type::refreshed:
      j["refreshed"] = token.refreshed();
      break;
    default:
      break;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const Token::Type& type) {
  j = boost::lexical_cast<std::string>(type);
}

void airmap::codec::json::encode(nlohmann::json& j, const Token::OAuth& token) {
  j["refresh_token"] = token.refresh;
  j["id_token"]      = token.id;
  j["access_token"]  = token.access;
  j["token_type"]    = token.type;
}

void airmap::codec::json::encode(nlohmann::json& j, const Token::OAuth::Type& type) {
  if (type == Token::OAuth::Type::bearer)
    j = "bearer";
}

void airmap::codec::json::encode(nlohmann::json& j, const Token::Anonymous& token) {
  j["id_token"] = token.id;
}

void airmap::codec::json::encode(nlohmann::json& j, const Token::Refreshed& token) {
  j["token_type"] = token.type;
  j["expires_in"] = token.expires_in.count();
  j["id_token"]   = token.id;
  if (token.original_token)
    j["original_token"] = token.original_token.get();
}
