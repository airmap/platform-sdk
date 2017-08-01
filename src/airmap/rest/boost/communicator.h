#ifndef AIRMAP_REST_BOOST_COMMUNICATOR_H_
#define AIRMAP_REST_BOOST_COMMUNICATOR_H_

#include <airmap/context.h>
#include <airmap/optional.h>
#include <airmap/rest/communicator.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <network/uri.hpp>

#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace airmap {
namespace rest {
namespace boost {

class Communicator : public airmap::rest::Communicator,
                     public airmap::Context,
                     public std::enable_shared_from_this<Communicator> {
 public:
  Communicator(const std::shared_ptr<Logger>& logger);
  ~Communicator();

  // From airmap::Context
  void create_client_with_credentials(const Client::Credentials& credentials, const ClientCreateCallback& cb) override;
  void run() override;
  void stop() override;

  // From airmap::rest::Communicator
  void get(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& query,
           std::unordered_map<std::string, std::string>&& headers, DoCallback cb) override;
  void post(const std::string& host, const std::string& path, std::unordered_map<std::string, std::string>&& headers,
            const std::string& body, DoCallback cb) override;
  void send_udp(const std::string& host, std::uint16_t port, const std::string& body) override;
  void dispatch(const std::function<void()>& task) override;

 private:
  struct HttpSession : public std::enable_shared_from_this<HttpSession> {
    struct Get {};
    struct Post {};
    explicit HttpSession(const Get&, const std::shared_ptr<Logger>& logger,
                         const std::shared_ptr<::boost::asio::io_service>& io_service,
                         const std::shared_ptr<::boost::asio::ssl::context>& ssl_context, const std::string& host,
                         const std::string& path, std::unordered_map<std::string, std::string>&& query,
                         std::unordered_map<std::string, std::string>&& headers, DoCallback cb);
    explicit HttpSession(const Post&, const std::shared_ptr<Logger>& logger,
                         const std::shared_ptr<::boost::asio::io_service>& io_service,
                         const std::shared_ptr<::boost::asio::ssl::context>& ssl_context, const std::string& host,
                         const std::string& path, const std::string& body,
                         std::unordered_map<std::string, std::string>&& headers, DoCallback cb);

    void start();
    void handle_resolve(const ::boost::system::error_code& error, ::boost::asio::ip::tcp::resolver::iterator iterator);
    void handle_connect(const ::boost::system::error_code& error);
    void handle_ssl_handshake(const ::boost::system::error_code& error);
    void handle_write(const ::boost::system::error_code& error);
    void handle_read(const ::boost::system::error_code& error);

    util::FormattingLogger log;
    std::shared_ptr<::boost::asio::io_service> io_service;
    std::shared_ptr<::boost::asio::ssl::context> ssl_context;
    ::boost::asio::ip::tcp::resolver resolver;
    ::boost::asio::ssl::stream<::boost::asio::ip::tcp::socket> socket;
    network::uri uri;
    ::boost::beast::http::request<::boost::beast::http::string_body> request;
    ::boost::beast::http::response<::boost::beast::http::string_body> response;
    ::boost::beast::flat_buffer buffer{8192};
    DoCallback cb;
  };

  // TODO(tvoss): We should think about refactoring the communicator interface in a way
  // that gets us rid of the host parameter to submit_udp calls. With that, we could easily
  // safe the resolver overhead per package submission.
  struct UdpSession : public std::enable_shared_from_this<UdpSession> {
    explicit UdpSession(const std::shared_ptr<Logger>& logger,
                        const std::shared_ptr<::boost::asio::io_service>& io_service, const std::string& host,
                        std::uint16_t port, const std::string& payload);

    void start();
    void handle_resolve(const ::boost::system::error_code& error, ::boost::asio::ip::udp::resolver::iterator iterator);
    void handle_write(const ::boost::system::error_code& error, std::size_t transferred);

    util::FormattingLogger log;
    std::shared_ptr<::boost::asio::io_service> io_service;
    ::boost::asio::ip::udp::resolver resolver;
    ::boost::asio::ip::udp::socket socket;
    network::uri uri;
    std::string payload;
  };

  util::FormattingLogger log_;

  std::shared_ptr<::boost::asio::io_service> io_service_;
  std::shared_ptr<::boost::asio::io_service::work> keep_alive_;
  std::shared_ptr<::boost::asio::ssl::context> ssl_context_;
};

}  // namespace boost
}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_BOOST_COMMUNICATOR_H_
