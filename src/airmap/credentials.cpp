//
//  credentials.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/credentials.h>

std::istream& airmap::operator>>(std::istream& in, Credentials::Type& type) {
  std::string s;
  in >> s;

  if (s == "anonymous")
    type = Credentials::Type::anonymous;
  else if (s == "oauth")
    type = Credentials::Type::oauth;

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, Credentials::Type type) {
  switch (type) {
    case Credentials::Type::anonymous:
      return out << "anonymous";
    case Credentials::Type::oauth:
      return out << "oauth";
  }

  return out;
}
