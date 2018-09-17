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
#include <airmap/codec/json/pilot.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::decode(const nlohmann::json& j, Pilot& p) {
  get(p.id, j, "id");
  get(p.first_name, j, "first_name");
  get(p.last_name, j, "last_name");
  get(p.user_name, j, "username");
  get(p.picture_url, j, "picture_url");
  get(p.created_at, j, "created_at");
  get(p.verification_status, j, "verification_status");
  get(p.statistics, j, "statistics");
  get(p.metadata.app, j, "app_metadata");
  get(p.metadata.user, j, "user_metadata");
}

void airmap::codec::json::decode(const nlohmann::json& j, Pilot::VerificationStatus& s) {
  get(s.email, j, "email");
  get(s.phone, j, "phone");
}

void airmap::codec::json::decode(const nlohmann::json& j, Pilot::Statistics& s) {
  get(s.flight, j, "flight");
  get(s.aircraft, j, "aircraft");
}

void airmap::codec::json::decode(const nlohmann::json& j, Pilot::Statistics::Flight& f) {
  get(f.total, j, "total");
  get(f.last_flight_time, j, "last_flight_time");
}

void airmap::codec::json::decode(const nlohmann::json& j, Pilot::Statistics::Aircraft& a) {
  get(a.total, j, "total");
}

void airmap::codec::json::decode(const nlohmann::json& j, Pilot::Aircraft& a) {
  get(a.id, j, "id");
  get(a.nick_name, j, "nickname");
  get(a.created_at, j, "created_at");
  get(a.model, j, "model");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Pilot::Aircraft>& v) {
  for (auto element : j) {
    v.push_back(Pilot::Aircraft{});
    v.back() = element;
  }
}
