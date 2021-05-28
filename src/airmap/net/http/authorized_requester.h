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
#ifndef AIRMAP_NET_HTTP_AUTHORIZED_REQUESTER_H_
#define AIRMAP_NET_HTTP_AUTHORIZED_REQUESTER_H_

#include <airmap/net/http/requester.h>
#include <airmap/util/formatting_logger.h>

#include <memory>
#include <string>

namespace airmap {
namespace net {
namespace http {

class AuthorizedRequester : public http::Requester {
 public:
  explicit AuthorizedRequester(const std::string& api_key, const std::shared_ptr<Requester>& next);

  void delete_(const std::string& path, std::unordered_map<std::string, std::string>&& query,
               std::unordered_map<std::string, std::string>&& headers, Callback cb) override;
  void get(const std::string& path, std::unordered_map<std::string, std::string>&& query,
           std::unordered_map<std::string, std::string>&& headers, Callback cb) override;
  void patch(const std::string& path, std::unordered_map<std::string, std::string>&& headers, const std::string& body,
             Callback cb) override;
  void post(const std::string& path, std::unordered_map<std::string, std::string>&& headers, const std::string& body,
            Callback cb) override;
  void set_auth_token(std::string token);

 private:
  std::string api_key_;
  Optional<std::string> auth_token_;
  std::shared_ptr<Requester> next_;
  util::FormattingLogger log_{create_null_logger()};
};

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_AUTHORIZED_REQUESTER_H_
