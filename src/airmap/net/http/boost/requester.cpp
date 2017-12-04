#include <airmap/net/http/boost/request.h>

#include <airmap/net/http/boost/requester.h>
#include <airmap/net/http/user_agent.h>

#include <network/uri.hpp>

#include <stdexcept>

namespace asio = boost::asio;
namespace http = boost::beast::http;
using tcp      = boost::asio::ip::tcp;

namespace {

airmap::Error wrap_error_code(const boost::system::error_code& ec) {
  return airmap::Error{ec.message()};
}

}  // namespace

airmap::net::http::boost::Requester::RequestFactory airmap::net::http::boost::Requester::request_factory_for_protocol(
    const std::string& protocol) {
  if (protocol == "http") {
    return non_encrypting_request_factory();
  } else if (protocol == "https") {
    return encrypting_request_factory();
  }

  throw std::logic_error{"unsupported protocol"};
}

airmap::net::http::boost::Requester::RequestFactory airmap::net::http::boost::Requester::encrypting_request_factory() {
  return [](const Request::Configuration& configuration) { return EncryptingRequest::create(configuration); };
}

airmap::net::http::boost::Requester::RequestFactory
airmap::net::http::boost::Requester::non_encrypting_request_factory() {
  return [](const Request::Configuration& configuration) { return NonEncryptingRequest::create(configuration); };
}

std::shared_ptr<airmap::net::http::boost::Requester> airmap::net::http::boost::Requester::create(
    const std::string& host, std::uint16_t port, const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<::boost::asio::io_service>& io_service, const RequestFactory& request_factory) {
  return std::shared_ptr<Requester>{new Requester{host, port, logger, io_service, request_factory}};
}

airmap::net::http::boost::Requester::Requester(const std::string& host, std::uint16_t port,
                                               const std::shared_ptr<Logger>& logger,
                                               const std::shared_ptr<::boost::asio::io_service>& io_service,
                                               const RequestFactory& request_factory)
    : log_{logger},
      io_service_{io_service},
      resolver_{*io_service_},
      host_{host},
      port_{port},
      request_factory_{request_factory} {
}

void airmap::net::http::boost::Requester::delete_(const std::string& path,
                                                  std::unordered_map<std::string, std::string>&& query,
                                                  std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  network::uri_builder uri_builder;
  for (const auto& pair : query)
    uri_builder.append_query_key_value_pair(pair.first, pair.second);
  auto uri = uri_builder.uri();
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::delete_);
  request.target(fmt::sprintf("%s?%s", path, uri.query()));
  request.set(::http::to_string(::http::field::user_agent), user_agent());
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive), [
    this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)
  ](const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
    if (ec) {
      cb(Result(wrap_error_code(ec)));
    } else {
      request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})->start();
    }
  });
}

void airmap::net::http::boost::Requester::get(const std::string& path,
                                              std::unordered_map<std::string, std::string>&& query,
                                              std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  network::uri_builder uri_builder;
  for (const auto& pair : query)
    uri_builder.append_query_key_value_pair(pair.first, pair.second);
  auto uri = uri_builder.uri();
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::get);
  request.target(fmt::sprintf("%s?%s", path, uri.query()));
  request.set(::http::to_string(::http::field::user_agent), user_agent());
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive), [
    this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)
  ](const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
    if (ec) {
      cb(Result(wrap_error_code(ec)));
    } else {
      request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})->start();
    }
  });
}

void airmap::net::http::boost::Requester::patch(const std::string& path,
                                                std::unordered_map<std::string, std::string>&& headers,
                                                const std::string& body, Callback cb) {
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::patch);
  request.target(path);
  request.set(::http::to_string(::http::field::user_agent), user_agent());
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::content_type), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.body = std::move(body);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive), [
    this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)
  ](const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
    if (ec) {
      cb(Result(wrap_error_code(ec)));
    } else {
      request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})->start();
    }
  });
}

void airmap::net::http::boost::Requester::post(const std::string& path,
                                               std::unordered_map<std::string, std::string>&& headers,
                                               const std::string& body, Callback cb) {
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::post);
  request.target(path);
  request.set(::http::to_string(::http::field::user_agent), user_agent());
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::content_type), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.body = std::move(body);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive), [
    this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)
  ](const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
    if (ec) {
      cb(Result(wrap_error_code(ec)));
    } else {
      request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})->start();
    }
  });
}
