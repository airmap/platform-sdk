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
#include <airmap/codec/http/query/elevation.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

#include <sstream>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Elevation::GetElevationPoints::Parameters& parameters) {
  // if (!parameters.points.empty()) {
  //   std::ostringstream oss;
  //   std::copy(parameters.points.begin(), parameters.points.end()-1, std::ostream_iterator<float>(oss, ","));
  //   oss << parameters.points.back();
  //   query["points"] = oss.str();
  // }
  if (parameters.points)
    query["points"] = parameters.points.get();
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Elevation::GetElevationCarpet::Parameters& parameters) {
  if (parameters.points)
    query["points"] = parameters.points.get();
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Elevation::GetElevationPath::Parameters& parameters) {
  if (parameters.points)
    query["points"] = parameters.points.get();
}