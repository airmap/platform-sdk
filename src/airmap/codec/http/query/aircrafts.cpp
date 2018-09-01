//
//  aircrafts.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/http/query/aircrafts.h>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Aircrafts::Manufacturers::Parameters& parameters) {
  if (parameters.manufacturer_name) {
    query["q"] = parameters.manufacturer_name.get();
  }
}

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Aircrafts::Models::Parameters& parameters) {
  if (parameters.manufacturer) {
    query["manufacturer"] = parameters.manufacturer.get().id;
  }
  if (parameters.model_name) {
    query["q"] = parameters.model_name.get();
  }
}