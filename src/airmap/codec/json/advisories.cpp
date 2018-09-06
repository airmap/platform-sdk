//
//  advisories.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/advisories.h>

#include <airmap/codec.h>
#include <airmap/codec/json/airspace.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::encode(nlohmann::json& j, const Advisory::Search::Parameters& p) {
  j["geometry"] = p.geometry;
  j["rulesets"] = p.rulesets;
  j["start"]    = p.start;
  j["end"]      = p.end;
}