#include <airmap/cmds/airmap/cmd/daemon.h>

#include <airmap/boost/context.h>
#include <airmap/mavlink/boost/serial_channel.h>
#include <airmap/mavlink/boost/tcp_channel.h>
#include <airmap/mavlink/boost/udp_channel.h>
#include <airmap/monitor/daemon.h>

#include <airmap/paths.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"daemon"};
}

cmd::Daemon::Daemon() : cli::CommandWithFlagsAndAction{"daemon", "runs the airmap daemon", "runs the airmap daemon"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(flags::telemetry_host(telemetry_host_));
  flag(flags::telemetry_port(telemetry_port_));
  flag(cli::make_flag("aircraft-id", "id of the device the daemon runs on", aircraft_id_));
  flag(cli::make_flag("grpc-endpoint", "grpc endpoint address", grpc_endpoint_));
  flag(cli::make_flag("serial-device", "the device file to read mavlink messages from", serial_device_));
  flag(cli::make_flag("tcp-endpoint-ip", "the ip of the tcp endpoint to read mavlink messages from", tcp_endpoint_ip_));
  flag(cli::make_flag("tcp-endpoint-port", "the port of the tcp endpoint to read mavlink messages from",
                      tcp_endpoint_port_));
  flag(cli::make_flag("udp-endpoint-port", "the port of the udp endpoint to read mavlink messages from",
                      udp_endpoint_port_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cout))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    if (telemetry_host_ && !telemetry_host_.get().validate()) {
      log_.errorf(component, "parameter 'telemetry-host' must not be empty");
      return 1;
    }

    bool has_valid_serial_device = serial_device_ && serial_device_.get().validate();
    bool has_valid_tcp_endpoint  = tcp_endpoint_ip_ && tcp_endpoint_ip_.get().validate() && tcp_endpoint_port_;
    bool has_valid_udp_endpoint  = static_cast<bool>(udp_endpoint_port_);

    if (!(has_valid_serial_device || has_valid_tcp_endpoint || has_valid_udp_endpoint)) {
      log_.errorf(component,
                  "neither a valid serial port, a valid tcp endpoint nor a valid udp endpoint was specified");
      return 1;
    }

    if (has_valid_serial_device && has_valid_tcp_endpoint && has_valid_udp_endpoint) {
      log_.errorf(component, "multiple mavlink endpoint options have been specified");
      return 1;
    }

    auto context = boost::Context::create(log_.logger());
    auto config  = Client::load_configuration_from_json(in_config);

    std::shared_ptr<mavlink::Channel> channel;

    if (has_valid_serial_device)
      channel =
          std::make_shared<mavlink::boost::SerialChannel>(log_.logger(), context->io_service(), serial_device_.get());
    if (has_valid_tcp_endpoint)
      channel = std::make_shared<mavlink::boost::TcpChannel>(
          log_.logger(), context->io_service(), ::boost::asio::ip::address::from_string(tcp_endpoint_ip_.get()),
          tcp_endpoint_port_.get());
    if (has_valid_udp_endpoint)
      channel =
          std::make_shared<mavlink::boost::UdpChannel>(log_.logger(), context->io_service(), udp_endpoint_port_.get());

    if (telemetry_host_)
      config.telemetry.host = telemetry_host_.get();
    if (telemetry_port_)
      config.telemetry.port = telemetry_port_.get();

    log_.infof(component,
               "configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  grpc endpoint:       %s\n"
               "  credentials.api_key: %s",
               config.host, config.version, config.telemetry.host, config.telemetry.port, grpc_endpoint_,
               config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, context, config, channel](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          ::airmap::monitor::Daemon::Configuration configuration{
              config.credentials, aircraft_id_, log_.logger(), channel, result.value(), grpc_endpoint_};

          ::airmap::monitor::Daemon::create(configuration)->start();
        });

    return context->exec({SIGINT, SIGQUIT},
                         [this, context](int sig) {
                           log_.infof(component, "received [%s], shutting down", ::strsignal(sig));
                           context->stop(::airmap::Context::ReturnCode::success);
                         }) == ::airmap::Context::ReturnCode::success
               ? 0
               : 1;
  });
}
