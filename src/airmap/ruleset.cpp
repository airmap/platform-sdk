//
//  ruleset.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/ruleset.h>

#include <iostream>
#include <unordered_map>

airmap::RuleSet::Feature::Value::Value() : type_{Type::unknown} {
}

airmap::RuleSet::Feature::Value::Value(bool value) : type_{Type::unknown} {
  construct(value);
}

airmap::RuleSet::Feature::Value::Value(double value) : type_{Type::unknown} {
  construct(value);
}

airmap::RuleSet::Feature::Value::Value(const std::string& value) : type_{Type::unknown} {
  construct(value);
}

airmap::RuleSet::Feature::Value::Value(const Value& other) : type_{Type::unknown} {
  construct(other);
}

airmap::RuleSet::Feature::Value::Value(Value&& other) : type_{Type::unknown} {
  construct(other);
}

airmap::RuleSet::Feature::Value::~Value() {
  destruct();
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::operator=(const Value& other) {
  return destruct().construct(other);
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::operator=(Value&& other) {
  return destruct().construct(other);
}

airmap::RuleSet::Feature::Type airmap::RuleSet::Feature::Value::type() const {
  return type_;
}

bool airmap::RuleSet::Feature::Value::boolean() const {
  return detail_.b;
}

double airmap::RuleSet::Feature::Value::floating_point() const {
  return detail_.d;
}

const std::string& airmap::RuleSet::Feature::Value::string() const {
  return detail_.s;
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::construct(const Value& other) {
  switch (other.type_) {
    case Type::boolean:
      construct(other.detail_.b);
      break;
    case Type::floating_point:
      construct(other.detail_.d);
      break;
    case Type::string:
      construct(other.detail_.s);
      break;
    case Type::unknown:
      destruct();
      break;
  }
  type_ = other.type_;
  return *this;
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::construct(Value&& other) {
  switch (other.type_) {
    case Type::boolean:
      construct(other.detail_.b);
      break;
    case Type::floating_point:
      construct(other.detail_.d);
      break;
    case Type::string:
      construct(other.detail_.s);
      break;
    case Type::unknown:
      destruct();
      break;
  }
  type_ = other.type_;
  return *this;
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::construct(bool value) {
  type_ = Type::boolean;
  new (&detail_.b) bool(value);
  return *this;
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::construct(double value) {
  type_ = Type::floating_point;
  new (&detail_.d) double(value);
  return *this;
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::construct(const std::string& value) {
  using sstring = std::string;
  type_         = Type::string;
  new (&detail_.s) sstring(value);
  return *this;
}

airmap::RuleSet::Feature::Value& airmap::RuleSet::Feature::Value::destruct() {
  using sstring = std::string;

  switch (type_) {
    case Type::boolean:
      break;
    case Type::floating_point:
      break;
    case Type::string:
      detail_.s.~sstring();
      break;
    default:
      break;
  }
  type_ = Type::unknown;
  return *this;
}

airmap::RuleSet::Feature::Value::Detail::Detail() : b{false} {
}
airmap::RuleSet::Feature::Value::Detail::~Detail() {
}

airmap::Optional<airmap::RuleSet::Feature::Value> airmap::RuleSet::Feature::value(bool b) const {
  if (type != Type::boolean)
    return Optional<Value>{};
  return Optional<Value>{Value{b}};
}

airmap::Optional<airmap::RuleSet::Feature::Value> airmap::RuleSet::Feature::value(double d) const {
  if (type != Type::floating_point)
    return Optional<Value>{};
  return Optional<Value>{Value{d}};
}

airmap::Optional<airmap::RuleSet::Feature::Value> airmap::RuleSet::Feature::value(const std::string& s) const {
  if (type != Type::boolean)
    return Optional<Value>{};
  return Optional<Value>{Value{s}};
}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::Feature::Type type) {
  switch (type) {
    case RuleSet::Feature::Type::boolean:
      return out << "bool";
    case RuleSet::Feature::Type::floating_point:
      return out << "float";
    case RuleSet::Feature::Type::string:
      return out << "string";
    case RuleSet::Feature::Type::unknown:
      return out << "unknown";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::Feature::Type& type) {
  std::string s;
  in >> s;

  if (s == "bool") {
    type = RuleSet::Feature::Type::boolean;
  } else if (s == "float") {
    type = RuleSet::Feature::Type::floating_point;
  } else if (s == "string") {
    type = RuleSet::Feature::Type::string;
  } else if (s == "unknown") {
    type = RuleSet::Feature::Type::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::Feature::Measurement measurement) {
  switch (measurement) {
    case RuleSet::Feature::Measurement::distance:
      return out << "distance";
    case RuleSet::Feature::Measurement::speed:
      return out << "speed";
    case RuleSet::Feature::Measurement::weight:
      return out << "weight";
    case RuleSet::Feature::Measurement::unknown:
      return out << "unknown";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::Feature::Measurement& measurement) {
  std::string s;
  in >> s;

  if (s == "distance") {
    measurement = RuleSet::Feature::Measurement::distance;
  } else if (s == "speed") {
    measurement = RuleSet::Feature::Measurement::speed;
  } else if (s == "weight") {
    measurement = RuleSet::Feature::Measurement::weight;
  } else if (s == "unknown") {
    measurement = RuleSet::Feature::Measurement::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::Feature::Unit unit) {
  switch (unit) {
    case RuleSet::Feature::Unit::kilograms:
      return out << "kilograms";
    case RuleSet::Feature::Unit::meters:
      return out << "meters";
    case RuleSet::Feature::Unit::meters_per_sec:
      return out << "meters_per_sec";
    case RuleSet::Feature::Unit::unknown:
      return out << "unknown";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::Feature::Unit& unit) {
  std::string s;
  in >> s;

  if (s == "kilograms") {
    unit = RuleSet::Feature::Unit::kilograms;
  } else if (s == "meters") {
    unit = RuleSet::Feature::Unit::meters;
  } else if (s == "meters_per_sec") {
    unit = RuleSet::Feature::Unit::meters_per_sec;
  } else if (s == "unknown") {
    unit = RuleSet::Feature::Unit::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::Jurisdiction::Region region) {
  switch (region) {
    case RuleSet::Jurisdiction::Region::national:
      return out << "national";
    case RuleSet::Jurisdiction::Region::state:
      return out << "state";
    case RuleSet::Jurisdiction::Region::county:
      return out << "county";
    case RuleSet::Jurisdiction::Region::city:
      return out << "city";
    case RuleSet::Jurisdiction::Region::local:
      return out << "local";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::Jurisdiction::Region& region) {
  std::string s;
  in >> s;

  if (s == "national" || s == "federal") {
    region = RuleSet::Jurisdiction::Region::national;
  } else if (s == "state") {
    region = RuleSet::Jurisdiction::Region::state;
  } else if (s == "county") {
    region = RuleSet::Jurisdiction::Region::county;
  } else if (s == "city") {
    region = RuleSet::Jurisdiction::Region::city;
  } else if (s == "local") {
    region = RuleSet::Jurisdiction::Region::local;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::SelectionType type) {
  switch (type) {
    case RuleSet::SelectionType::optional:
      return out << "optional";
    case RuleSet::SelectionType::required:
      return out << "required";
    case RuleSet::SelectionType::pickone:
      return out << "pickone";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::SelectionType& type) {
  std::string s;
  in >> s;

  if (s == "optional") {
    type = RuleSet::SelectionType::optional;
  } else if (s == "required") {
    type = RuleSet::SelectionType::required;
  } else if (s == "pickone") {
    type = RuleSet::SelectionType::pickone;
  } else if (s == "pick1") {
    type = RuleSet::SelectionType::pickone;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, RuleSet::Rule::Status status) {
  switch (status) {
    case RuleSet::Rule::Status::unknown:
      return out << "unknown";
    case RuleSet::Rule::Status::conflicting:
      return out << "conflicting";
    case RuleSet::Rule::Status::not_conflicting:
      return out << "not_conflicting";
    case RuleSet::Rule::Status::missing_info:
      return out << "missing_info";
    case RuleSet::Rule::Status::informational:
      return out << "informational";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, RuleSet::Rule::Status& status) {
  std::string s;
  in >> s;

  if (s == "conflicting") {
    status = RuleSet::Rule::Status::conflicting;
  } else if (s == "not_conflicting") {
    status = RuleSet::Rule::Status::not_conflicting;
  } else if (s == "missing_info") {
    status = RuleSet::Rule::Status::missing_info;
  } else if (s == "informational" || s == "informational_rules") {
    status = RuleSet::Rule::Status::informational;
  } else {
    status = RuleSet::Rule::Status::unknown;
  }

  return in;
}
