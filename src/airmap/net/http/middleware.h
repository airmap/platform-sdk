//
//  middleware.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_NET_HTTP_MIDDLEWARE_H_
#define AIRMAP_NET_HTTP_MIDDLEWARE_H_

#include <airmap/jsend.h>
#include <airmap/outcome.h>

#include <airmap/net/http/requester.h>

#include <functional>

namespace airmap {
namespace net {
namespace http {

template <typename T>
Requester::Callback jsend_parsing_request_callback(const std::function<void(const Outcome<T, Error>&)>& next) {
  return [next](const Outcome<Response, Error>& result) {
    if (result) {
      const auto& response = result.value();

      switch (response.classify()) {
        case Response::Classification::informational:
        case Response::Classification::redirection:
          break;
        case Response::Classification::success:
        case Response::Classification::client_error:
        case Response::Classification::server_error:
          next(jsend::parse_to_outcome<T>(result.value().body));
          break;
        default:
          next(Outcome<T, Error>{Error{"networking error"}
                                     .value(Error::Value{std::string{"status-code"}},
                                            Error::Value{static_cast<std::int64_t>(result.value().status)})
                                     .value(Error::Value{std::string{"body"}}, Error::Value{result.value().body})});
          break;
      }
    } else {
      next(Outcome<T, Error>{result.error()});
    }
  };
}

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_MIDDLEWARE_H_