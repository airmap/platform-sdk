#include <airmap/flight_plan.h>

#include <iostream>

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
