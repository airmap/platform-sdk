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
#include <airmap/net/http/requester_with_api_key.h>

airmap::net::http::RequesterWithApiKey::RequesterWithApiKey(const std::string& api_key,
                                                            const std::shared_ptr<Requester>& next)
    : api_key_{api_key}, next_{next} {
}

void airmap::net::http::RequesterWithApiKey::delete_(const std::string& path,
                                                     std::unordered_map<std::string, std::string>&& query,
                                                     std::unordered_map<std::string, std::string>&& headers,
                                                     Callback cb) {
  headers["X-API-Key"] = api_key_;
  next_->delete_(path, std::move(query), std::move(headers), cb);
}
void airmap::net::http::RequesterWithApiKey::get(const std::string& path,
                                                 std::unordered_map<std::string, std::string>&& query,
                                                 std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  headers["X-API-Key"] = api_key_;
  next_->get(path, std::move(query), std::move(headers), cb);
}
void airmap::net::http::RequesterWithApiKey::patch(const std::string& path,
                                                   std::unordered_map<std::string, std::string>&& headers,
                                                   const std::string& body, Callback cb) {
  headers["X-API-Key"] = api_key_;
  next_->patch(path, std::move(headers), std::move(body), cb);
}
void airmap::net::http::RequesterWithApiKey::post(const std::string& path,
                                                  std::unordered_map<std::string, std::string>&& headers,
                                                  const std::string& body, Callback cb) {
  headers["X-API-Key"] = api_key_;
  next_->post(path, std::move(headers), std::move(body), cb);
}
