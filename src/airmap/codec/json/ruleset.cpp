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
#include <airmap/codec/json/ruleset.h>

#include <airmap/codec.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet& r) {
  get(r.id, j, "id");
  get(r.selection_type, j, "selection_type");
  get(r.name, j, "name");
  get(r.short_name, j, "short_name");
  get(r.description, j, "description");
  get(r.is_default, j, "default");
  get(r.jurisdiction, j, "jurisdiction");
  get(r.airspace_types, j, "airspace_types");
  get(r.rules, j, "rules");
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Rule& r) {
  get(r.short_text, j, "short_text");
  get(r.description, j, "description");
  get(r.status, j, "status");
  get(r.display_order, j, "display_order");
  get(r.features, j, "flight_features");
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet::Rule>& v) {
  for (auto element : j) {
    v.push_back(RuleSet::Rule{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature& f) {
  get(f.id, j, "id");
  get(f.code, j, "code");
  get(f.name, j, "flight_feature");
  get(f.description, j, "description");
  get(f.status, j, "status");
  get(f.type, j, "input_type");
  get(f.measurement, j, "measurement_type");
  get(f.unit, j, "measurement_unit");
  get(f.is_calculated, j, "is_calculated");
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Type& t) {
  try {
    t = boost::lexical_cast<RuleSet::Feature::Type>(j.get<std::string>());
  } catch(boost::bad_lexical_cast const& e) {
      t = RuleSet::Feature::Type::unknown;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Measurement& m) {
    try {
        m = boost::lexical_cast<RuleSet::Feature::Measurement>(j.get<std::string>());
    } catch(boost::bad_lexical_cast const& e) {
        m = RuleSet::Feature::Measurement::unknown;
    }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Unit& u) {
  try {
    u = boost::lexical_cast<RuleSet::Feature::Unit>(j.get<std::string>());
  } catch(boost::bad_lexical_cast const& e) {
        u = RuleSet::Feature::Unit::unknown;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Feature::Value& v) {
  switch (j.type()) {
    case nlohmann::json::value_t::boolean:
      v = RuleSet::Feature::Value{j.get<bool>()};
      break;
    case nlohmann::json::value_t::string:
      v = RuleSet::Feature::Value{j.get<std::string>()};
      break;
    case nlohmann::json::value_t::number_integer:
    case nlohmann::json::value_t::number_unsigned:
    case nlohmann::json::value_t::number_float:
      v = RuleSet::Feature::Value{j.get<double>()};
      break;
    default:
      v = RuleSet::Feature::Value{};
      break;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet::Feature>& v) {
  for (auto element : j) {
    v.push_back(RuleSet::Feature{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Rule::Status& s) {
  s = boost::lexical_cast<RuleSet::Rule::Status>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, std::vector<RuleSet>& v) {
  for (auto element : j) {
    v.push_back(RuleSet{});
    v.back() = element;
  }
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::SelectionType& t) {
  t = boost::lexical_cast<RuleSet::SelectionType>(j.get<std::string>());
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Jurisdiction& jd) {
  get(jd.id, j, "id");
  get(jd.name, j, "name");
  get(jd.region, j, "region");
}

void airmap::codec::json::decode(const nlohmann::json& j, RuleSet::Jurisdiction::Region& r) {
  r = boost::lexical_cast<RuleSet::Jurisdiction::Region>(j.get<std::string>());
}

void airmap::codec::json::encode(nlohmann::json& j, const RuleSet::Feature::Value& v) {
  switch (v.type()) {
    case RuleSet::Feature::Type::boolean:
      j = v.boolean();
      break;
    case RuleSet::Feature::Type::floating_point:
      j = v.floating_point();
      break;
    case RuleSet::Feature::Type::string:
      j = v.string();
      break;
    default:
      break;
  }
}

void airmap::codec::json::encode(nlohmann::json& j, const std::vector<RuleSet::Id>& v) {
  for (const auto& id : v) {
    j.push_back(id);
  }
}
