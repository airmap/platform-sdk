#include <airmap/client.h>

#include <iostream>

airmap::Client::Configuration airmap::Client::default_production_configuration(const Credentials& credentials) {
  return Configuration{"api.airmap.com",
                       Version::production,
                       {"sso.airmap.io", 443},
                       {"api-udp-telemetry.airmap.com", 16060},
                       {"mqtt-prod.airmap.io", 8883},
                       credentials};
}

airmap::Client::Configuration airmap::Client::default_staging_configuration(const Credentials& credentials) {
  return Configuration{"api.airmap.com",
                       Version::staging,
                       {"sso.airmap.io", 443},
                       {"k8s.stage.airmap.com", 16060},
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