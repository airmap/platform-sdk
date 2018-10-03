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
#ifndef AIRMAP_CODEC_H_
#define AIRMAP_CODEC_H_

#include <airmap/codec/http/query/advisories.h>
#include <airmap/codec/http/query/aircrafts.h>
#include <airmap/codec/http/query/airspaces.h>
#include <airmap/codec/http/query/flights.h>
#include <airmap/codec/http/query/pilots.h>
#include <airmap/codec/http/query/rulesets.h>
#include <airmap/codec/http/query/status.h>

#if defined(AIRMAP_ENABLE_GRPC)
#include <airmap/codec/grpc/date_time.h>
#include <airmap/codec/grpc/traffic.h>
#endif  // AIRMAP_ENABLE_GRPC

#include <airmap/codec/json/advisories.h>
#include <airmap/codec/json/advisory.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/airspace.h>
#include <airmap/codec/json/authenticator.h>
#include <airmap/codec/json/chrono.h>
#include <airmap/codec/json/client.h>
#include <airmap/codec/json/credentials.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/evaluation.h>
#include <airmap/codec/json/flight.h>
#include <airmap/codec/json/flight_plan.h>
#include <airmap/codec/json/flight_plans.h>
#include <airmap/codec/json/flights.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>
#include <airmap/codec/json/pilot.h>
#include <airmap/codec/json/pilots.h>
#include <airmap/codec/json/rule.h>
#include <airmap/codec/json/ruleset.h>
#include <airmap/codec/json/rulesets.h>
#include <airmap/codec/json/status.h>
#include <airmap/codec/json/token.h>
#include <airmap/codec/json/traffic.h>

namespace airmap {

template <typename T>
inline void from_json(const nlohmann::json& j, T& value) {
  codec::json::decode(j, value);
}

template <typename T>
inline void to_json(nlohmann::json& j, const T& value) {
  codec::json::encode(j, value);
}

}  // namespace airmap

#endif  // AIRMAP_CODEC_H_
