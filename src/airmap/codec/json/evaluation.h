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
#ifndef AIRMAP_CODEC_JSON_EVALUATION_H_
#define AIRMAP_CODEC_JSON_EVALUATION_H_

#include <airmap/evaluation.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Evaluation& e);
void decode(const nlohmann::json& j, Evaluation::Authority& a);
void decode(const nlohmann::json& j, Evaluation::Authorization& a);
void decode(const nlohmann::json& j, std::vector<Evaluation::Authorization>& v);
void decode(const nlohmann::json& j, Evaluation::Authorization::Status& s);
void decode(const nlohmann::json& j, Evaluation::Validation& v);
void decode(const nlohmann::json& j, std::vector<Evaluation::Validation>& v);
void decode(const nlohmann::json& j, Evaluation::Validation::Feature& f);
void decode(const nlohmann::json& j, Evaluation::Validation::Status& s);
void decode(const nlohmann::json& j, Evaluation::Failure& f);
void decode(const nlohmann::json& j, std::vector<Evaluation::Failure>& v);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_EVALUATION_H_
