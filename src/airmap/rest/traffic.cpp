#include <airmap/rest/traffic.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <nlohmann/json.hpp>

#include <mqtt_client_cpp.hpp>

// TBD - separate UUID creator class

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>             // uuid class
#include <boost/uuid/uuid_generators.hpp>  // generators
#include <boost/uuid/uuid_io.hpp>          // streaming operators etc.

using json = nlohmann::json;

airmap::rest::Traffic::Traffic(Communicator& communicator) : communicator_{communicator} {
}

void airmap::rest::Traffic::subscribe(const Subscribe::Params& params, const Subscribe::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  // TBD - test only

  boost::asio::io_service ios;

  std::uint16_t pid_sub1;
  std::uint16_t pid_sub2;

  std::string flightID = params.flight_id;  //"flight|RNmw7D8hPpkePkS5G8B62sWONK3D";
  std::string token =
      params
          .token;  //"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJodHRwczovL3Nzby5haXJtYXAuaW8vIiwic3ViIjoiYW5vbnw5ODA4NmM3N2NlN2ZiZmIzZDk1MTI1YzI2ZTA1NmNhYzRkZjM5NDM3NDJmMzY3NDVhMWU0Njg4M2MwY2Y5YjEyIiwiYXVkIjoiMmlWMVhTZmRMSk5PZlppVFo5Skdkck5IdGNOellzdHQiLCJleHAiOjE1MDI3MDQyMzQsImlhdCI6MTUwMjY2ODIzNCwic2NwcyI6WyJjcmVhdGU6ZmxpZ2h0IiwiZmxpZ2h0LWNvbW1zIl19.K54__x07uRQOu--wOQesYbjRwtRItReQkwi9hbZy_TY";
  std::string topic_alert = "uav/traffic/alert/" + flightID;
  std::string topic_sa    = "uav/traffic/sa/" + flightID;

  int count = 0;
  // Create TLS client
  auto c = mqtt::make_tls_client(ios, "mqtt-prod.airmap.io", 8883);

  // Setup client
  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  c->set_client_id(boost::lexical_cast<std::string>(uuid));
  c->set_clean_session(true);
  c->set_ca_cert_file("am.crt");
  c->set_user_name(flightID);
  c->set_password(token);
  // c->set_keep_alive_sec(10);

  // Setup handlers
  c->set_connack_handler([&c, &pid_sub1, &pid_sub2, topic_sa, topic_alert](bool sp, std::uint8_t connack_return_code) {
    std::cout << "Connack handler called" << std::endl;
    std::cout << "Clean Session: " << std::boolalpha << sp << std::endl;
    std::cout << "Connack Return Code: " << mqtt::connect_return_code_to_str(connack_return_code) << std::endl;
    if (connack_return_code == mqtt::connect_return_code::accepted) {
      pid_sub1 = c->subscribe(topic_sa, mqtt::qos::at_most_once);
      pid_sub2 = c->subscribe(topic_alert, mqtt::qos::at_most_once);
    }
    return true;
  });
  c->set_close_handler([]() { std::cout << "closed." << std::endl; });
  c->set_error_handler(
      [](boost::system::error_code const& ec) { std::cout << "error: " << ec.message() << std::endl; });
  c->set_puback_handler([&c](std::uint16_t packet_id) {
    std::cout << "puback received. packet_id: " << packet_id << std::endl;
    return true;
  });
  c->set_pubrec_handler([&c](std::uint16_t packet_id) {
    std::cout << "pubrec received. packet_id: " << packet_id << std::endl;
    return true;
  });
  c->set_pubcomp_handler([&c](std::uint16_t packet_id) {
    std::cout << "pubcomp received. packet_id: " << packet_id << std::endl;
    return true;
  });
  bool first = true;
  c->set_suback_handler(
      [&c, &first, &pid_sub1, &pid_sub2](std::uint16_t packet_id, std::vector<boost::optional<std::uint8_t>> results) {
        std::cout << "suback received. packet_id: " << packet_id << std::endl;
        for (auto const& e : results) {
          if (e) {
            std::cout << "subscribe success: " << mqtt::qos::to_str(*e) << std::endl;
          } else {
            std::cout << "subscribe failed" << std::endl;
          }
        }
        return true;
      });

  c->set_publish_handler([&c, &count](std::uint8_t header, boost::optional<std::uint16_t> packet_id,
                                      std::string topic_name, std::string contents) {
    std::cout << "publish received. "
              << "dup: " << std::boolalpha << mqtt::publish::is_dup(header)
              << " pos: " << mqtt::qos::to_str(mqtt::publish::get_qos(header))
              << " retain: " << mqtt::publish::is_retain(header) << std::endl;
    if (packet_id)
      std::cout << "packet_id: " << *packet_id << std::endl;
    std::cout << "topic_name: " << topic_name << std::endl;
    std::cout << "contents: " << contents << std::endl;
    return true;
  });

  // Connect
  c->connect();

  ios.run();

  cb(jsend::to_outcome<TrafficInfo>(json::parse("{}")));  // TBD
}
