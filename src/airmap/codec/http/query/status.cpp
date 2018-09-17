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
#include <airmap/codec/http/query/status.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

#include <sstream>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Status::GetStatus::Parameters& parameters) {
  if (parameters.latitude)
    query["latitude"] = boost::lexical_cast<std::string>(parameters.latitude);
  if (parameters.longitude)
    query["longitude"] = boost::lexical_cast<std::string>(parameters.longitude);
  if (parameters.types && parameters.types.get() != Airspace::Type::invalid) {
    query["types"] =
        boost::lexical_cast<std::string>((parameters.types.get() & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
  }
  if (parameters.ignored_types && parameters.ignored_types.get() != Airspace::Type::invalid) {
    query["ignored_types"] = boost::lexical_cast<std::string>(
        (parameters.ignored_types.get() & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
  }
  if (parameters.flight_date_time)
    query["datetime"] = iso8601::generate(parameters.flight_date_time.get());
  if (parameters.geometry) {
    nlohmann::json geometry;
    geometry          = parameters.geometry.get();
    query["geometry"] = geometry.dump();
  }
  if (parameters.buffer)
    query["buffer"] = boost::lexical_cast<std::string>(parameters.buffer.get());
  if (parameters.weather)
    query["weather"] = parameters.weather.get() ? "true" : "false";
}
