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
#include <airmap/codec/http/query/pilots.h>

#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Pilots::Authenticated::Parameters& parameters) {
  if (parameters.exclude)
    query["enhance"] = boost::lexical_cast<std::string>(parameters.exclude.get());
  query["statistics"] = parameters.retrieve_statistics ? "true" : "false";
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Pilots::ForId::Parameters& parameters) {
  if (parameters.exclude)
    query["enhance"] = boost::lexical_cast<std::string>(parameters.exclude.get());
  query["statistics"] = parameters.retrieve_statistics ? "true" : "false";
}
