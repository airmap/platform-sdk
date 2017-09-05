#ifndef AIRMAP_NET_HTTP_REQUESTER_H_
#define AIRMAP_NET_HTTP_REQUESTER_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <airmap/net/http/response.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace airmap {
namespace net {
namespace http {

class Requester : DoNotCopyOrMove {
 public:
  using Result   = Outcome<Response, std::exception_ptr>;
  using Callback = std::function<void(const Result&)>;

  virtual void delete_(const std::string& path, std::unordered_map<std::string, std::string>&& query,
                       std::unordered_map<std::string, std::string>&& headers, Callback cb) = 0;
  virtual void get(const std::string& path, std::unordered_map<std::string, std::string>&& query,
                   std::unordered_map<std::string, std::string>&& headers, Callback cb) = 0;
  virtual void patch(const std::string& path, std::unordered_map<std::string, std::string>&& headers,
                     const std::string& body, Callback cb) = 0;
  virtual void post(const std::string& path, std::unordered_map<std::string, std::string>&& headers,
                    const std::string& body, Callback cb) = 0;

 protected:
  Requester() = default;
};

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_REQUESTER_H_
