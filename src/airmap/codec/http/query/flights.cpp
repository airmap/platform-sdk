//
//  flights.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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
