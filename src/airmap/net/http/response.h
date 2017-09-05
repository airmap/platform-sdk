#ifndef AIRMAP_NET_HTTP_RESPONSE_H_
#define AIRMAP_NET_HTTP_RESPONSE_H_

#include <string>
#include <unordered_map>

namespace airmap {
namespace net {
namespace http {

struct Response {
  unsigned int version;
  unsigned int status;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_RESPONSE_H_
