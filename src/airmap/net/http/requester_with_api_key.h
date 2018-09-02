//
//  requester_with_api_key.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_NET_HTTP_REQUESTER_WITH_API_KEY_H_
#define AIRMAP_NET_HTTP_REQUESTER_WITH_API_KEY_H_

#include <airmap/net/http/requester.h>

#include <memory>
#include <string>

namespace airmap {
namespace net {
namespace http {

class RequesterWithApiKey : public http::Requester {
 public:
  explicit RequesterWithApiKey(const std::string& api_key, const std::shared_ptr<Requester>& next);

  void delete_(const std::string& path, std::unordered_map<std::string, std::string>&& query,
               std::unordered_map<std::string, std::string>&& headers, Callback cb) override;
  void get(const std::string& path, std::unordered_map<std::string, std::string>&& query,
           std::unordered_map<std::string, std::string>&& headers, Callback cb) override;
  void patch(const std::string& path, std::unordered_map<std::string, std::string>&& headers, const std::string& body,
             Callback cb) override;
  void post(const std::string& path, std::unordered_map<std::string, std::string>&& headers, const std::string& body,
            Callback cb) override;

 private:
  std::string api_key_;
  std::shared_ptr<Requester> next_;
};

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_REQUESTER_WITH_API_KEY_H_
