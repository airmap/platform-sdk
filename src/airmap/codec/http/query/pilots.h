//
//  pilots.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_HTTP_QUERY_PILOTS_H_
#define AIRMAP_CODEC_HTTP_QUERY_PILOTS_H_

#include <airmap/pilots.h>

#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

void encode(std::unordered_map<std::string, std::string>& query, const Pilots::Authenticated::Parameters& parameters);
void encode(std::unordered_map<std::string, std::string>& query, const Pilots::ForId::Parameters& parameters);

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_PILOTS_H_
