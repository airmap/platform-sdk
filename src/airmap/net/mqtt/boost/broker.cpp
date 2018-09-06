//
//  broker.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/net/mqtt/boost/broker.h>

#include <airmap/net/mqtt/boost/client.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <mqtt/str_connect_return_code.hpp>

#include <stdexcept>

namespace uuids = boost::uuids;

airmap::net::mqtt::boost::Broker::Broker(const std::string& host, std::uint16_t port,
                                         const std::shared_ptr<Logger>& logger,
                                         const std::shared_ptr<::boost::asio::io_service>& io_service)
    : logger_{logger}, io_service_{io_service}, host_{host}, port_{port} {
}

void airmap::net::mqtt::boost::Broker::connect(const Credentials& credentials, const ConnectCallback& cb) {
  auto client = ::mqtt::make_tls_client(*io_service_, host_, std::to_string(port_));
  client->set_clean_session(true);
  client->set_default_verify_paths();
  client->set_client_id(uuids::to_string(uuids::random_generator()()));
  client->set_user_name(credentials.username);
  client->set_password(credentials.password);

  client->set_connack_handler(
      [logger = logger_, io_service = io_service_, host = host_, port = port_, cb, client](auto, auto rc) {
        if (::mqtt::connect_return_code::accepted == rc) {
          cb(ConnectResult(Client::create(logger, io_service, client)));
        } else {
          cb(ConnectResult{Error{"failed to connect to mqtt broker"}
                               .description(::mqtt::connect_return_code_to_str(rc))
                               .value(Error::Value{"host"}, Error::Value{host})
                               .value(Error::Value{"port"}, Error::Value{static_cast<std::int64_t>(port)})});
        }
        return ::mqtt::connect_return_code::accepted == rc;
      });

  client->connect([cb](const auto& ec) {
    if (ec) {
      cb(ConnectResult{Error{ec.message()}});
    }
  });
}
