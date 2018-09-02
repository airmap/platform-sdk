//
//  evaluation.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/evaluation.h>

#include <iostream>
#include <unordered_map>

std::ostream& airmap::operator<<(std::ostream& out, Evaluation::Authorization::Status status) {
  switch (status) {
    case Evaluation::Authorization::Status::accepted:
      return out << "accepted";
    case Evaluation::Authorization::Status::rejected:
      return out << "rejected";
    case Evaluation::Authorization::Status::pending:
      return out << "pending";
    case Evaluation::Authorization::Status::accepted_upon_submission:
      return out << "accepted_upon_submission";
    case Evaluation::Authorization::Status::rejected_upon_submission:
      return out << "rejected_upon_submission";
  }

  return out;
}

std::istream& airmap::operator>>(std::istream& in, Evaluation::Authorization::Status& status) {
  std::string s;
  in >> s;

  if (s == "accepted") {
    status = Evaluation::Authorization::Status::accepted;
  } else if (s == "rejected") {
    status = Evaluation::Authorization::Status::rejected;
  } else if (s == "pending") {
    status = Evaluation::Authorization::Status::pending;
  } else if (s == "accepted_upon_submission") {
    status = Evaluation::Authorization::Status::accepted_upon_submission;
  } else if (s == "rejected_upon_submission") {
    status = Evaluation::Authorization::Status::rejected_upon_submission;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, Evaluation::Validation::Status status) {
  switch (status) {
    case Evaluation::Validation::Status::valid:
      return out << "valid";
    case Evaluation::Validation::Status::invalid:
      return out << "invalid";
    case Evaluation::Validation::Status::unknown:
      return out << "unknown";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, Evaluation::Validation::Status& status) {
  std::string s;
  in >> s;

  if (s == "valid") {
    status = Evaluation::Validation::Status::valid;
  } else if (s == "invalid") {
    status = Evaluation::Validation::Status::invalid;
  } else if (s == "unknown") {
    status = Evaluation::Validation::Status::unknown;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, Evaluation::Failure failure) {
  switch (failure) {
    case Evaluation::Failure::validation:
      return out << "validation";
    case Evaluation::Failure::authorization:
      return out << "authorization";
    case Evaluation::Failure::rulesets:
      return out << "rulesets";
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, Evaluation::Failure& failure) {
  std::string s;
  in >> s;

  if (s == "validation") {
    failure = Evaluation::Failure::validation;
  } else if (s == "authorization") {
    failure = Evaluation::Failure::authorization;
  } else if (s == "rulesets") {
    failure = Evaluation::Failure::rulesets;
  }

  return in;
}
