#include <airmap/net/http/boost/request.h>
#include <airmap/net/http/boost/requester.h>

#include <network/uri.hpp>

#include <stdexcept>

namespace asio = boost::asio;
namespace http = boost::beast::http;
using tcp      = boost::asio::ip::tcp;

namespace {

std::exception_ptr wrap_error_code(const boost::system::error_code& ec) {
  return std::make_exception_ptr(std::runtime_error{ec.message()});
}

}  // namespace

std::shared_ptr<airmap::net::http::boost::Requester> airmap::net::http::boost::Requester::create(
    const std::string& host, std::uint16_t port, const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<::boost::asio::io_service>& io_service) {
  return std::shared_ptr<Requester>{new Requester{host, port, logger, io_service}};
}

airmap::net::http::boost::Requester::Requester(const std::string& host, std::uint16_t port,
                                               const std::shared_ptr<Logger>& logger,
                                               const std::shared_ptr<::boost::asio::io_service>& io_service)
    : log_{logger}, io_service_{io_service}, resolver_{*io_service_}, host_{host}, port_{port} {
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
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
                          [ this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb) ](
                              const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
                            if (ec) {
                              cb(Result(wrap_error_code(ec)));
                            } else {
                              Request::create(log_.logger(), io_service_, *iterator, request, std::move(cb))->start();
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
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
                          [ this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb) ](
                              const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
                            if (ec) {
                              cb(Result(wrap_error_code(ec)));
                            } else {
                              Request::create(log_.logger(), io_service_, *iterator, request, std::move(cb))->start();
                            }
                          });
}

void airmap::net::http::boost::Requester::patch(const std::string& path,
                                                std::unordered_map<std::string, std::string>&& headers,
                                                const std::string& body, Callback cb) {
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::patch);
  request.target(path);
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::content_type), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.body = std::move(body);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
                          [ this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb) ](
                              const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
                            if (ec) {
                              cb(Result(wrap_error_code(ec)));
                            } else {
                              Request::create(log_.logger(), io_service_, *iterator, request, std::move(cb))->start();
                            }
                          });
}

void airmap::net::http::boost::Requester::post(const std::string& path,
                                               std::unordered_map<std::string, std::string>&& headers,
                                               const std::string& body, Callback cb) {
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::post);
  request.target(path);
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::content_type), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.body = std::move(body);
  request.prepare_payload();

  resolver_.async_resolve(tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
                          [ this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb) ](
                              const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
                            if (ec) {
                              cb(Result(wrap_error_code(ec)));
                            } else {
                              Request::create(log_.logger(), io_service_, *iterator, request, std::move(cb))->start();
                            }
                          });
}