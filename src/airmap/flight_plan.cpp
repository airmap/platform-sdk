#include <airmap/flight_plan.h>

#include <iostream>
#include <unordered_map>

airmap::FlightPlan::Briefing::Feature::Value::Value() : type_{Type::unknown} {
}

airmap::FlightPlan::Briefing::Feature::Value::Value(bool value) : type_{Type::unknown} {
  construct(value);
}

airmap::FlightPlan::Briefing::Feature::Value::Value(double value) : type_{Type::unknown} {
  construct(value);
}

airmap::FlightPlan::Briefing::Feature::Value::Value(const std::string& value) : type_{Type::unknown} {
  construct(value);
}

airmap::FlightPlan::Briefing::Feature::Value::Value(const Value& other) : type_{Type::unknown} {
  construct(other);
}

airmap::FlightPlan::Briefing::Feature::Value::Value(Value&& other) : type_{Type::unknown} {
  construct(other);
}

airmap::FlightPlan::Briefing::Feature::Value::~Value() {
  destruct();
}

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::operator=(
    const Value& other) {
  return destruct().construct(other);
}

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::operator=(Value&& other) {
  return destruct().construct(other);
}

airmap::FlightPlan::Briefing::Feature::Type airmap::FlightPlan::Briefing::Feature::Value::type() const {
  return type_;
}

bool airmap::FlightPlan::Briefing::Feature::Value::boolean() const {
  return detail_.b;
}

double airmap::FlightPlan::Briefing::Feature::Value::floating_point() const {
  return detail_.d;
}

const std::string& airmap::FlightPlan::Briefing::Feature::Value::string() const {
  return detail_.s;
}

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::construct(
    const Value& other) {
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

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::construct(Value&& other) {
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

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::construct(bool value) {
  type_ = Type::boolean;
  new (&detail_.b) bool(value);
  return *this;
}

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::construct(double value) {
  type_ = Type::floating_point;
  new (&detail_.d) double(value);
  return *this;
}

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::construct(
    const std::string& value) {
  using sstring = std::string;
  type_         = Type::string;
  new (&detail_.s) sstring(value);
  return *this;
}

airmap::FlightPlan::Briefing::Feature::Value& airmap::FlightPlan::Briefing::Feature::Value::destruct() {
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

airmap::FlightPlan::Briefing::Feature::Value::Detail::Detail() : b{false} {
}
airmap::FlightPlan::Briefing::Feature::Value::Detail::~Detail() {
}

airmap::Optional<airmap::FlightPlan::Briefing::Feature::Value> airmap::FlightPlan::Briefing::Feature::value(
    bool b) const {
  if (type != Type::boolean)
    return Optional<Value>{};
  return Optional<Value>{Value{b}};
}

airmap::Optional<airmap::FlightPlan::Briefing::Feature::Value> airmap::FlightPlan::Briefing::Feature::value(
    double d) const {
  if (type != Type::floating_point)
    return Optional<Value>{};
  return Optional<Value>{Value{d}};
}

airmap::Optional<airmap::FlightPlan::Briefing::Feature::Value> airmap::FlightPlan::Briefing::Feature::value(
    const std::string& s) const {
  if (type != Type::boolean)
    return Optional<Value>{};
  return Optional<Value>{Value{s}};
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::Feature::Type type) {
  switch (type) {
    case FlightPlan::Briefing::Feature::Type::boolean:
      return out << "bool";
    case FlightPlan::Briefing::Feature::Type::floating_point:
      return out << "float";
    case FlightPlan::Briefing::Feature::Type::string:
      return out << "string";
    case FlightPlan::Briefing::Feature::Type::unknown:
      return out << "unknown";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::Feature::Type& type) {
  std::string s;
  in >> s;

  if (s == "bool") {
    type = FlightPlan::Briefing::Feature::Type::boolean;
  } else if (s == "float") {
    type = FlightPlan::Briefing::Feature::Type::floating_point;
  } else if (s == "string") {
    type = FlightPlan::Briefing::Feature::Type::string;
  } else if (s == "unknown") {
    type = FlightPlan::Briefing::Feature::Type::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::Feature::Measurement measurement) {
  switch (measurement) {
    case FlightPlan::Briefing::Feature::Measurement::distance:
      return out << "distance";
    case FlightPlan::Briefing::Feature::Measurement::speed:
      return out << "speed";
    case FlightPlan::Briefing::Feature::Measurement::weight:
      return out << "weight";
    case FlightPlan::Briefing::Feature::Measurement::unknown:
      return out << "unknown";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::Feature::Measurement& measurement) {
  std::string s;
  in >> s;

  if (s == "distance") {
    measurement = FlightPlan::Briefing::Feature::Measurement::distance;
  } else if (s == "speed") {
    measurement = FlightPlan::Briefing::Feature::Measurement::speed;
  } else if (s == "weight") {
    measurement = FlightPlan::Briefing::Feature::Measurement::weight;
  } else if (s == "unknown") {
    measurement = FlightPlan::Briefing::Feature::Measurement::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::Feature::Unit unit) {
  switch (unit) {
    case FlightPlan::Briefing::Feature::Unit::kilograms:
      return out << "kilograms";
    case FlightPlan::Briefing::Feature::Unit::meters:
      return out << "meters";
    case FlightPlan::Briefing::Feature::Unit::meters_per_sec:
      return out << "meters_per_sec";
    case FlightPlan::Briefing::Feature::Unit::unknown:
      return out << "unknown";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::Feature::Unit& unit) {
  std::string s;
  in >> s;

  if (s == "kilograms") {
    unit = FlightPlan::Briefing::Feature::Unit::kilograms;
  } else if (s == "meters") {
    unit = FlightPlan::Briefing::Feature::Unit::meters;
  } else if (s == "meters_per_sec") {
    unit = FlightPlan::Briefing::Feature::Unit::meters_per_sec;
  } else if (s == "unknown") {
    unit = FlightPlan::Briefing::Feature::Unit::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::RuleSet::Type type) {
  switch (type) {
    case FlightPlan::Briefing::RuleSet::Type::optional:
      return out << "optional";
    case FlightPlan::Briefing::RuleSet::Type::required:
      return out << "required";
    case FlightPlan::Briefing::RuleSet::Type::pickone:
      return out << "pickone";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::RuleSet::Type& type) {
  std::string s;
  in >> s;

  if (s == "optional") {
    type = FlightPlan::Briefing::RuleSet::Type::optional;
  } else if (s == "required") {
    type = FlightPlan::Briefing::RuleSet::Type::required;
  } else if (s == "pickone") {
    type = FlightPlan::Briefing::RuleSet::Type::pickone;
  } else if (s == "pick1") {
    type = FlightPlan::Briefing::RuleSet::Type::pickone;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::RuleSet::Rule::Status status) {
  switch (status) {
    case FlightPlan::Briefing::RuleSet::Rule::Status::unknown:
      return out << "unknown";
    case FlightPlan::Briefing::RuleSet::Rule::Status::conflicting:
      return out << "conflicting";
    case FlightPlan::Briefing::RuleSet::Rule::Status::not_conflicting:
      return out << "not_conflicting";
    case FlightPlan::Briefing::RuleSet::Rule::Status::missing_info:
      return out << "missing_info";
    case FlightPlan::Briefing::RuleSet::Rule::Status::informational:
      return out << "informational";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::RuleSet::Rule::Status& status) {
  std::string s;
  in >> s;

  if (s == "conflicting") {
    status = FlightPlan::Briefing::RuleSet::Rule::Status::conflicting;
  } else if (s == "not_conflicting") {
    status = FlightPlan::Briefing::RuleSet::Rule::Status::not_conflicting;
  } else if (s == "missing_info") {
    status = FlightPlan::Briefing::RuleSet::Rule::Status::missing_info;
  } else if (s == "informational" || s == "informational_rules") {
    status = FlightPlan::Briefing::RuleSet::Rule::Status::informational;
  } else {
    status = FlightPlan::Briefing::RuleSet::Rule::Status::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::Jurisdiction::Region region) {
  switch (region) {
    case FlightPlan::Briefing::Jurisdiction::Region::national:
      return out << "national";
    case FlightPlan::Briefing::Jurisdiction::Region::state:
      return out << "state";
    case FlightPlan::Briefing::Jurisdiction::Region::county:
      return out << "county";
    case FlightPlan::Briefing::Jurisdiction::Region::city:
      return out << "city";
    case FlightPlan::Briefing::Jurisdiction::Region::local:
      return out << "local";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::Jurisdiction::Region& region) {
  std::string s;
  in >> s;

  if (s == "national" || s == "federal") {
    region = FlightPlan::Briefing::Jurisdiction::Region::national;
  } else if (s == "state") {
    region = FlightPlan::Briefing::Jurisdiction::Region::state;
  } else if (s == "county") {
    region = FlightPlan::Briefing::Jurisdiction::Region::county;
  } else if (s == "city") {
    region = FlightPlan::Briefing::Jurisdiction::Region::city;
  } else if (s == "local") {
    region = FlightPlan::Briefing::Jurisdiction::Region::local;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::Authorization::Status status) {
  switch (status) {
    case FlightPlan::Briefing::Authorization::Status::accepted:
      return out << "accepted";
    case FlightPlan::Briefing::Authorization::Status::rejected:
      return out << "rejected";
    case FlightPlan::Briefing::Authorization::Status::pending:
      return out << "pending";
    case FlightPlan::Briefing::Authorization::Status::accepted_upon_submission:
      return out << "accepted_upon_submission";
    case FlightPlan::Briefing::Authorization::Status::rejected_upon_submission:
      return out << "rejected_upon_submission";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::Authorization::Status& status) {
  std::string s;
  in >> s;

  if (s == "accepted") {
    status = FlightPlan::Briefing::Authorization::Status::accepted;
  } else if (s == "rejected") {
    status = FlightPlan::Briefing::Authorization::Status::rejected;
  } else if (s == "pending") {
    status = FlightPlan::Briefing::Authorization::Status::pending;
  } else if (s == "accepted_upon_submission") {
    status = FlightPlan::Briefing::Authorization::Status::accepted_upon_submission;
  } else if (s == "rejected_upon_submission") {
    status = FlightPlan::Briefing::Authorization::Status::rejected_upon_submission;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, FlightPlan::Briefing::Validation::Status status) {
  switch (status) {
    case FlightPlan::Briefing::Validation::Status::valid:
      return out << "valid";
    case FlightPlan::Briefing::Validation::Status::invalid:
      return out << "invalid";
    case FlightPlan::Briefing::Validation::Status::unknown:
      return out << "unknown";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, FlightPlan::Briefing::Validation::Status& status) {
  std::string s;
  in >> s;

  if (s == "valid") {
    status = FlightPlan::Briefing::Validation::Status::valid;
  } else if (s == "invalid") {
    status = FlightPlan::Briefing::Validation::Status::invalid;
  } else if (s == "unknown") {
    status = FlightPlan::Briefing::Validation::Status::unknown;
  }

  return in;
}
