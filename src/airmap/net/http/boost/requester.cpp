// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <airmap/net/http/boost/request.h>

#include <airmap/net/http/boost/requester.h>
#include <airmap/net/http/user_agent.h>

#include <iterator>
#include <stdexcept>

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace fmt  = airmap::util::fmt;
using tcp      = boost::asio::ip::tcp;

namespace {

airmap::Error wrap_error_code(const boost::system::error_code& ec) {
  return airmap::Error{ec.message()};
}

namespace uri {

template <typename CharT>
inline CharT hex_to_letter(CharT in) {
  if ((in >= 0) && (in < 10)) {
    return in + '0';
  }

  if ((in >= 10) && (in < 16)) {
    return in - 10 + 'A';
  }

  return in;
}

template <class charT, class OutputIterator>
void encode_char(charT in, OutputIterator& out, const char* ignore = "") {
  if (((in >= 'a') && (in <= 'z')) || ((in >= 'A') && (in <= 'Z')) || ((in >= '0') && (in <= '9')) || (in == '-') ||
      (in == '.') || (in == '_') || (in == '~')) {
    out++ = in;
  } else {
    auto first = ignore, last = ignore + std::strlen(ignore);
    if (std::find(first, last, in) != last) {
      out++ = in;
    } else {
      out++ = '%';
      out++ = hex_to_letter((in >> 4) & 0x0f);
      out++ = hex_to_letter(in & 0x0f);
    }
  }
}

template <typename InputIterator, typename OutputIterator>
OutputIterator encode_query(InputIterator first, InputIterator last, OutputIterator out) {
  auto it = first;
  while (it != last) {
    encode_char(*it, out, "/.@&%;=");
    ++it;
  }
  return out;
}

std::string encode_query(const std::unordered_map<std::string, std::string>& q) {
  std::stringstream ss;

  for (auto it = q.begin(); it != q.end(); ++it) {
    if (it != q.begin())
      ss << "&";

    auto p = it->first + "=" + it->second;
    encode_query(p.begin(), p.end(), std::ostream_iterator<char>(ss));
  }

  return ss.str();
}

}  // namespace uri
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
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::delete_);
  request.target(fmt::sprintf("%s?%s", path, uri::encode_query(query)));
  request.set(::http::to_string(::http::field::user_agent), user_agent());
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();

  resolver_.async_resolve(
      tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
      [this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)](
          const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
        if (ec) {
          cb(Result(wrap_error_code(ec)));
        } else {
          request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})
              ->start();
        }
      });
}

void airmap::net::http::boost::Requester::get(const std::string& path,
                                              std::unordered_map<std::string, std::string>&& query,
                                              std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  ::boost::beast::http::request<::boost::beast::http::string_body> request;
  request.method(::http::verb::get);
  request.target(fmt::sprintf("%s?%s", path, uri::encode_query(query)));
  request.set(::http::to_string(::http::field::user_agent), user_agent());
  request.set(::http::to_string(::http::field::accept), "application/json");
  request.set(::http::to_string(::http::field::host), host_);
  for (const auto& pair : headers)
    request.set(pair.first, pair.second);
  request.prepare_payload();

  resolver_.async_resolve(
      tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
      [this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)](
          const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
        if (ec) {
          cb(Result(wrap_error_code(ec)));
        } else {
          request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})
              ->start();
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
  request.body() = std::move(body);
  request.prepare_payload();

  resolver_.async_resolve(
      tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
      [this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)](
          const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
        if (ec) {
          cb(Result(wrap_error_code(ec)));
        } else {
          request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})
              ->start();
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
  request.body() = std::move(body);
  request.prepare_payload();

  resolver_.async_resolve(
      tcp::resolver::query(host_, std::to_string(port_), tcp::resolver::query::passive),
      [this, sp = shared_from_this(), request = std::move(request), cb = std::move(cb)](
          const ::boost::system::error_code& ec, tcp::resolver::iterator iterator) {
        if (ec) {
          cb(Result(wrap_error_code(ec)));
        } else {
          request_factory_(Request::Configuration{log_.logger(), io_service_, *iterator, request, std::move(cb)})
              ->start();
        }
      });
}
