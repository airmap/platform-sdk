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
