/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  evaluation.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/flight_plan.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>
#include <airmap/codec/json/ruleset.h>

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation& e) {
  get(e.rulesets, j, "rulesets");
  get(e.validations, j, "validations");
  get(e.authorizations, j, "authorizations");
  get(e.failures, j, "failures");
}

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation::Authority& a) {
  get(a.name, j, "name");
  get(a.id, j, "id");
}

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation::Authorization& a) {
  get(a.status, j, "status");
  get(a.authority, j, "authority");
  get(a.message, j, "message");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Evaluation::Authorization>& v) {
  for (auto element : j) {
    v.push_back(Evaluation::Authorization{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation::Authorization::Status& s) {
  s = boost::lexical_cast<Evaluation::Authorization::Status>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation::Validation& v) {
  get(v.status, j, "status");
  get(v.data, j, "data");
  get(v.message, j, "message");
  get(v.feature, j, "feature");
  get(v.authority, j, "authority");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Evaluation::Validation>& v) {
  for (auto element : j) {
    v.push_back(Evaluation::Validation{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation::Validation::Feature& f) {
  get(f.code, j, "code");
  get(f.description, j, "description");
}

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation::Validation::Status& s) {
  s = boost::lexical_cast<Evaluation::Validation::Status>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, Evaluation::Failure& f) {
  f = boost::lexical_cast<Evaluation::Failure>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Evaluation::Failure>& v) {
  for (auto element : j) {
    v.push_back(Evaluation::Failure{});
    v.back() = element;
  }
}
