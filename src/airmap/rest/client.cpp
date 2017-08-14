#include <airmap/rest/client.h>

airmap::rest::Client::Client(const Credentials& credentials, const std::shared_ptr<Communicator>& communicator)
    : credentials_{credentials},
      communicator_{communicator},
      aircrafts_{*this},
      airspaces_{*this},
      authenticator_{*this},
      flights_{*this},
      telemetry_{*this},
      traffic_{*this} {
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

airmap::Flights& airmap::rest::Client::flights() {
  return flights_;
}

airmap::Telemetry& airmap::rest::Client::telemetry() {
  return telemetry_;
}

airmap::Traffic& airmap::rest::Client::traffic() {
  return traffic_;
}

void airmap::rest::Client::get(const std::string& host, const std::string& path,
                               std::unordered_map<std::string, std::string>&& query,
                               std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  headers["X-API-Key"] = credentials_.api_key;
  communicator_->get(host, path, std::move(query), std::move(headers), std::move(cb));
}

void airmap::rest::Client::post(const std::string& host, const std::string& path,
                                std::unordered_map<std::string, std::string>&& headers, const std::string& body,
                                DoCallback cb) {
  headers["X-API-Key"] = credentials_.api_key;
  communicator_->post(host, path, std::move(headers), body, std::move(cb));
}

void airmap::rest::Client::send_udp(const std::string& host, std::uint16_t port, const std::string& body) {
  communicator_->send_udp(host, port, body);
}

void airmap::rest::Client::dispatch(const std::function<void()>& task) {
  communicator_->dispatch(task);
}
