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
void encode(nlohmann::json& j, const Token& t);
void encode(nlohmann::json& j, const Token::Type& t);
void encode(nlohmann::json& j, const Token::Anonymous& a);
void encode(nlohmann::json& j, const Token::OAuth& o);
void encode(nlohmann::json& j, const Token::OAuth::Type& o);
void encode(nlohmann::json& j, const Token::Refreshed& r);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_TOKEN_H_
