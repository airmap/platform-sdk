
#ifndef AIRMAP_CODEC_JSON_TOKEN_H_
#define AIRMAP_CODEC_JSON_TOKEN_H_

#include <airmap/token.h>

#include <nlohmann/json.hpp>

#include <chrono>
#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Token& t);
void decode(const nlohmann::json& j, Token::Type& t);
void decode(const nlohmann::json& j, Token::Anonymous& a);
void decode(const nlohmann::json& j, Token::OAuth& o);
void decode(const nlohmann::json& j, Token::OAuth::Type& t);
void decode(const nlohmann::json& j, Token::Refreshed& r);
void decode(const nlohmann::json& j, Token::Refreshed::Type& t);
void encode(nlohmann::json& j, const Token& t);
void encode(nlohmann::json& j, const Token::Type& t);
void encode(nlohmann::json& j, const Token::Anonymous& a);
void encode(nlohmann::json& j, const Token::OAuth& o);
void encode(nlohmann::json& j, const Token::OAuth::Type& o);
void encode(nlohmann::json& j, const Token::Refreshed& r);
void encode(nlohmann::json& j, const Token::Refreshed::Type& t);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_TOKEN_H_
