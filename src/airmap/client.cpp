//
//  client.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/client.h>

#include <airmap/codec.h>

#include <iostream>

using json = nlohmann::json;

airmap::Client::Configuration airmap::Client::default_production_configuration(const Credentials& credentials) {
  return Configuration{"api.airmap.com",
                       Version::production,
                       {"sso.airmap.io", 443},
                       {"api.k8s.stage.airmap.com", 32003},
                       {"mqtt-prod.airmap.io", 8883},
                       credentials};
}

airmap::Client::Configuration airmap::Client::default_staging_configuration(const Credentials& credentials) {
  return Configuration{"api.airmap.com",
                       Version::staging,
                       {"sso.airmap.io", 443},
                       {"api.k8s.stage.airmap.com", 32003},
                       {"mqtt-stage.airmap.io", 8883},
                       credentials};
}

airmap::Client::Configuration airmap::Client::default_configuration(Client::Version version,
                                                                    const Credentials& credentials) {
  switch (version) {
    case Client::Version::production:
      return default_production_configuration(credentials);
    case Client::Version::staging:
      return default_staging_configuration(credentials);
  }
  throw std::logic_error{"should not reach here"};
}

airmap::Client::Configuration airmap::Client::load_configuration_from_json(std::istream& in) {
  if (!in) {
    throw std::runtime_error{"failed to read from stream"};
  }

  Configuration configuration = json::parse(in);
  return configuration;
}

std::istream& airmap::operator>>(std::istream& in, Client::Version& version) {
  std::string s;
  in >> s;

  if (s == "production") {
    version = Client::Version::production;
  } else if (s == "staging") {
    version = Client::Version::staging;
  }

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, Client::Version version) {
  switch (version) {
    case Client::Version::production:
      return out << "production";
    case Client::Version::staging:
      return out << "staging";
  }

  return out;
}
