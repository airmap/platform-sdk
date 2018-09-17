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
#include <airmap/codec/http/query/flights.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Flights::ForId::Parameters& parameters) {
  if (parameters.enhance)
    query["enhance"] = parameters.enhance.get() ? "true" : "false";
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Flights::Search::Parameters& parameters) {
  if (parameters.limit)
    query["limit"] = boost::lexical_cast<std::string>(parameters.limit.get());
  if (parameters.geometry) {
    nlohmann::json geometry;
    geometry          = parameters.geometry.get();
    query["geometry"] = geometry.dump();
  }
  if (parameters.country)
    query["country"] = parameters.country.get();
  if (parameters.state)
    query["state"] = parameters.state.get();
  if (parameters.city)
    query["city"] = parameters.city.get();
  if (parameters.pilot_id)
    query["pilot_id"] = parameters.pilot_id.get();
  if (parameters.start_after)
    query["start_after"] = iso8601::generate(parameters.start_after.get());
  if (parameters.start_before)
    query["start_before"] = iso8601::generate(parameters.start_before.get());
  if (parameters.end_after)
    query["end_after"] = iso8601::generate(parameters.end_after.get());
  if (parameters.end_before)
    query["end_before"] = iso8601::generate(parameters.end_before.get());
  if (parameters.enhance)
    query["enhance"] = parameters.enhance.get() ? "true" : "false";
}
