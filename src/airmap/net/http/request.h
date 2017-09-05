#ifndef AIRMAP_NET_HTTP_REQUEST_H_
#define AIRMAP_NET_HTTP_REQUEST_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/net/http/response.h>
#include <airmap/outcome.h>

#include <functional>
#include <stdexcept>
#include <string>

namespace airmap {
namespace net {
namespace http {

class Request : DoNotCopyOrMove {
 public:
  using Result   = Outcome<Response, std::exception_ptr>;
  using Callback = std::function<void(const Result&)>;

  virtual void execute(const Callback& cb) = 0;

 protected:
  Request() = default;
};

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_REQUEST_H_