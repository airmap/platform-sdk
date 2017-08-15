#ifndef AIRMAP_REST_CLIENT_H_
#define AIRMAP_REST_CLIENT_H_

#include <airmap/client.h>

#include <airmap/rest/aircrafts.h>
#include <airmap/rest/airspaces.h>
#include <airmap/rest/authenticator.h>
#include <airmap/rest/communicator.h>
#include <airmap/rest/flights.h>
#include <airmap/rest/pilots.h>
#include <airmap/rest/status.h>
#include <airmap/rest/telemetry.h>
#include <airmap/rest/traffic.h>

#include <memory>

namespace airmap {
namespace rest {

class Client : public airmap::Client, public airmap::rest::Communicator {
 public:
  explicit Client(const Configuration& configuration, const std::shared_ptr<Logger>& logger,
                  const std::shared_ptr<Communicator>& communicator);

  // From airmap::Client
  airmap::Aircrafts& aircrafts() override;
  airmap::Airspaces& airspaces() override;
  airmap::Authenticator& authenticator() override;
  airmap::Flights& flights() override;
  airmap::Pilots& pilots() override;
  airmap::Status& status() override;
  airmap::Telemetry& telemetry() override;
  airmap::Traffic& traffic() override;

  // From airmap::rest::Communicator
  void connect_to_mqtt_broker(const std::string& host, std::uint16_t port, const std::string& username,
                              const std::string& password, const ConnectCallback& cb) override;
  void delete_(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& query,
               std::unordered_map<std::string, std::string>&& headers, DoCallback cb) override;
  void get(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& query,
           std::unordered_map<std::string, std::string>&& headers, DoCallback cb) override;
  void post(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& headers,
            const std::string& body, DoCallback cb) override;
  void patch(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& headers,
             const std::string& body, DoCallback cb) override;
  void send_udp(const std::string& host, std::uint16_t port, const std::string& body) override;
  void dispatch(const std::function<void()>& task) override;

 private:
  Configuration configuration_;
  std::shared_ptr<rest::Communicator> communicator_;

  rest::Aircrafts aircrafts_;
  rest::Airspaces airspaces_;
  rest::Authenticator authenticator_;
  rest::Flights flights_;
  rest::Pilots pilots_;
  rest::Status status_;
  rest::Telemetry telemetry_;
  rest::Traffic traffic_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_CLIENT_H_
