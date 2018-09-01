//
//  rulesets.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/http/query/rulesets.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

#include <sstream>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const RuleSets::FetchRules::Parameters& parameters) {
  if (parameters.rulesets)
    query["rulesets"] = parameters.rulesets.get();
}
