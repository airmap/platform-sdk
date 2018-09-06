//
//  aircrafts.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_HTTP_QUERY_AIRCRAFTS_H_
#define AIRMAP_CODEC_HTTP_QUERY_AIRCRAFTS_H_

#include <airmap/aircrafts.h>

#include <unordered_map>
#include <vector>

namespace airmap {
namespace codec {
namespace http {
namespace query {

void encode(std::unordered_map<std::string, std::string>& query,
            const Aircrafts::Manufacturers::Parameters& parameters);
void encode(std::unordered_map<std::string, std::string>& query, const Aircrafts::Models::Parameters& parameters);

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_AIRCRAFTS_H_
