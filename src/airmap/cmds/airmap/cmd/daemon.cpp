#include <airmap/cmds/airmap/cmd/daemon.h>

#include <airmap/daemon.h>
#include <airmap/mavlink/boost/serial_channel.h>
#include <airmap/mavlink/boost/tcp_channel.h>
#include <airmap/rest/boost/communicator.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"daemon"};
}

cmd::Daemon::Daemon()
    : cli::CommandWithFlagsAndAction{cli::Name{"daemon"}, cli::Usage{"runs the airmap daemon"},
                                     cli::Description{"runs the airmap daemon"}} {
  flag(cli::make_flag(cli::Name{"version"}, cli::Description{"work against this version of the AirMap services"},
                      version_));
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"user-id"}, cli::Description{"user-id for authenticating with the AirMap services"},
                      user_id_));
  flag(cli::make_flag(cli::Name{"aircraft-id"}, cli::Description{"id of the device the daemon runs on"}, aircraft_id_));
  flag(cli::make_flag(cli::Name{"serial-device"}, cli::Description{"the device file to read mavlink messages from"},
                      serial_device_));
  flag(cli::make_flag(cli::Name{"telemetry-host"}, cli::Description{"telemetry host address"}, telemetry_host_));
  flag(cli::make_flag(cli::Name{"telemetry-port"}, cli::Description{"telemetry host port"}, telemetry_port_));
  flag(cli::make_flag(cli::Name{"tcp-endpoint-ip"},
                      cli::Description{"the ip of the tcp endpoint to read mavlink messages from"}, tcp_endpoint_ip_));
  flag(cli::make_flag(cli::Name{"tcp-endpoint-port"},
                      cli::Description{"the port of the tcp endpoint to read mavlink messages from"},
                      tcp_endpoint_port_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_default_logger(ctxt.cout)};

    if (!api_key_) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!api_key_.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!user_id_) {
      log_.errorf(component, "missing parameter 'user-id'");
      return 1;
    }

    if (!user_id_.get().validate()) {
      log_.errorf(component, "parameter 'user-id' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!aircraft_id_) {
      log_.errorf(component, "missing parameter 'aircraft-id'");
      return 1;
    }

    if (!aircraft_id_.get().validate()) {
      log_.errorf(component, "parameter 'aircraft-id' for accessing AirMap services must not be empty");
      return 1;
    }

    if (telemetry_host_ && !telemetry_host_.get().validate()) {
      log_.errorf(component, "parameter 'telemetry-host' must not be empty");
      return 1;
    }

    bool has_valid_serial_device = serial_device_ && serial_device_.get().validate();
    bool has_valid_tcp_endpoint  = tcp_endpoint_ip_ && tcp_endpoint_ip_.get().validate() && tcp_endpoint_port_;

    if (!(has_valid_serial_device || has_valid_tcp_endpoint)) {
      log_.errorf(component, "neither a valid serial port nor a valid tcp endpoint was specified");
      return 1;
    }

    if (has_valid_serial_device && has_valid_tcp_endpoint) {
      log_.errorf(component, "both a serial port and a tcp endpoint were specified");
      return 1;
    }

    auto communicator = std::make_shared<rest::boost::Communicator>(log_.logger());

    std::shared_ptr<mavlink::Channel> channel;

    if (has_valid_serial_device)
      channel = std::make_shared<mavlink::boost::SerialChannel>(log_.logger(), communicator->io_service(),
                                                                serial_device_.get());
    if (has_valid_tcp_endpoint)
      channel = std::make_shared<mavlink::boost::TcpChannel>(
          log_.logger(), communicator->io_service(), boost::asio::ip::address::from_string(tcp_endpoint_ip_.get()),
          tcp_endpoint_port_.get());

    auto config = Client::default_configuration(version_, Client::Credentials{api_key_.get()});

    if (telemetry_host_)
      config.telemetry.host = telemetry_host_.get();
    if (telemetry_port_)
      config.telemetry.port = telemetry_port_.get();

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    communicator->create_client_with_configuration(
        config, [this, communicator, channel](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          ::airmap::Daemon::Configuration configuration{api_key_.get(), user_id_.get(), aircraft_id_.get(),
                                                        log_.logger(),  channel,        result.value()};

          ::airmap::Daemon::create(configuration)->start();
        });

    communicator->run();

    return 0;
  });
}
