//
//  rule.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/json/rule.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void airmap::codec::json::decode(const nlohmann::json& j, Rule& rule) {
  get(rule.type, j, "type");
  get(rule.id, j, "id");
  get(rule.name, j, "name");
  get(rule.description, j, "description");
  get(rule.jurisdiction, j, "jurisdiction");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<Rule>& v) {
  for (auto element : j) {
    v.push_back(Rule{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json&, Rule::Type&) {
}