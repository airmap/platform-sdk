#include <airmap/rest/client.h>

airmap::rest::Client::Client(const Configuration& configuration, const std::shared_ptr<Context>& parent,
                             const std::shared_ptr<net::udp::Sender>& sender,
                             const std::shared_ptr<net::http::Requester>& airmap_requester,
                             const std::shared_ptr<net::http::Requester>& sso_requester,
                             const std::shared_ptr<net::mqtt::Broker>& broker)
    : configuration_{configuration},
      parent_{parent},
      udp_sender_{sender},
      http_{std::make_shared<airmap::net::http::RequesterWithApiKey>(configuration_.credentials.api_key,
                                                                     airmap_requester),
            sso_requester},
      mqtt_broker_{broker},
      aircrafts_{configuration_.version, http_.airmap_requester},
      airspaces_{configuration_.version, http_.airmap_requester},
      authenticator_{configuration_.version, http_.airmap_requester, http_.sso_requester},
      flight_plans_{configuration_.version, http_.airmap_requester},
      flights_{configuration_.version, http_.airmap_requester},
      pilots_{configuration_.version, http_.airmap_requester},
      rulesets_{configuration_.version, http_.airmap_requester},
      status_{configuration_.version, http_.airmap_requester},
      telemetry_{std::make_shared<detail::OpenSSLAES256Encryptor>(), udp_sender_},
      traffic_{mqtt_broker_} {
}

airmap::Aircrafts& airmap::rest::Client::aircrafts() {
  return aircrafts_;
}

airmap::Airspaces& airmap::rest::Client::airspaces() {
  return airspaces_;
}

airmap::Authenticator& airmap::rest::Client::authenticator() {
  return authenticator_;
}

airmap::FlightPlans& airmap::rest::Client::flight_plans() {
  return flight_plans_;
}

airmap::Flights& airmap::rest::Client::flights() {
  return flights_;
}

airmap::Pilots& airmap::rest::Client::pilots() {
  return pilots_;
}

airmap::RuleSets& airmap::rest::Client::rulesets() {
  return rulesets_;
}

airmap::Status& airmap::rest::Client::status() {
  return status_;
}

airmap::Telemetry& airmap::rest::Client::telemetry() {
  return telemetry_;
}

airmap::Traffic& airmap::rest::Client::traffic() {
  return traffic_;
}
