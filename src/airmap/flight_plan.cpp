#include <airmap/flight_plan.h>

#include <iostream>
#include <unordered_map>

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
