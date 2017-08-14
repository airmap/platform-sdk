#include <airmap/rest/client.h>

airmap::rest::Client::Client(const Configuration& configuration, const std::shared_ptr<Logger>& logger,
                             const std::shared_ptr<Communicator>& communicator)
    : configuration_{configuration},
      communicator_{communicator},
      aircrafts_{configuration_.host, configuration_.version, *this},
      airspaces_{configuration_.host, configuration_.version, *this},
      authenticator_{configuration_.host, configuration_.version, *this},
      flights_{configuration_.host, configuration_.version, *this},
      pilots_{configuration_.host, configuration_.version, *this},
      telemetry_{configuration_.telemetry.host, configuration_.telemetry.port, *this},
      traffic_{logger, configuration_.traffic.host, configuration_.traffic.port, *this} {
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

airmap::Pilots& airmap::rest::Client::pilots() {
  return pilots_;
}

airmap::Telemetry& airmap::rest::Client::telemetry() {
  return telemetry_;
}

void airmap::rest::Client::connect_to_mqtt_broker(const std::string& host, std::uint16_t port,
                                                  const std::string& username, const std::string& password,
                                                  const ConnectCallback& cb) {
  communicator_->connect_to_mqtt_broker(host, port, username, password, cb);
}

void airmap::rest::Client::delete_(const std::string& host, const std::string& path,
                                   std::unordered_map<std::string, std::string>&& query,
                                   std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  headers["X-API-Key"] = configuration_.credentials.api_key;
  communicator_->delete_(host, path, std::move(query), std::move(headers), std::move(cb));
}

airmap::Traffic& airmap::rest::Client::traffic() {
  return traffic_;
}

void airmap::rest::Client::get(const std::string& host, const std::string& path,
                               std::unordered_map<std::string, std::string>&& query,
                               std::unordered_map<std::string, std::string>&& headers, DoCallback cb) {
  headers["X-API-Key"] = configuration_.credentials.api_key;
  communicator_->get(host, path, std::move(query), std::move(headers), std::move(cb));
}

void airmap::rest::Client::post(const std::string& host, const std::string& path,
                                std::unordered_map<std::string, std::string>&& headers, const std::string& body,
                                DoCallback cb) {
  headers["X-API-Key"] = configuration_.credentials.api_key;
  communicator_->post(host, path, std::move(headers), body, std::move(cb));
}

void airmap::rest::Client::patch(const std::string& host, const std::string& path,
                                 std::unordered_map<std::string, std::string>&& headers, const std::string& body,
                                 DoCallback cb) {
  headers["X-API-Key"] = configuration_.credentials.api_key;
  communicator_->patch(host, path, std::move(headers), body, std::move(cb));
}

void airmap::rest::Client::send_udp(const std::string& host, std::uint16_t port, const std::string& body) {
  communicator_->send_udp(host, port, body);
}

void airmap::rest::Client::dispatch(const std::function<void()>& task) {
  communicator_->dispatch(task);
}