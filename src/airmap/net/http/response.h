//
//  response.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_NET_HTTP_RESPONSE_H_
#define AIRMAP_NET_HTTP_RESPONSE_H_

#include <string>
#include <unordered_map>

namespace airmap {
namespace net {
namespace http {

struct Response {
  // Classification enumerates all known classes of response statuses.
  enum class Classification {
    informational = 100,
    success       = 200,
    redirection   = 300,
    client_error  = 400,
    server_error  = 500,
    unclassified  = 999
  };

  // classify returns the Classification of the status of this response.
  Classification classify() const;

  unsigned int version;
  unsigned int status;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_RESPONSE_H_
