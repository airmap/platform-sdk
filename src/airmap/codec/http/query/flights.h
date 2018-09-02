//
//  flights.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_HTTP_QUERY_FLIGHTS_H_
#define AIRMAP_CODEC_HTTP_QUERY_FLIGHTS_H_

#include <airmap/flights.h>

#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

void encode(std::unordered_map<std::string, std::string>& query, const Flights::ForId::Parameters& parameters);
void encode(std::unordered_map<std::string, std::string>& query, const Flights::Search::Parameters& parameters);

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_FLIGHTS_H_
