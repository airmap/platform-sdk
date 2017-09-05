#ifndef AIRMAP_NET_UDP_SENDER_H_
#define AIRMAP_NET_UDP_SENDER_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <functional>
#include <stdexcept>
#include <string>

namespace airmap {
namespace net {
namespace udp {

class Sender : DoNotCopyOrMove {
 public:
  struct Empty {};
  using Result   = Outcome<Empty, std::exception_ptr>;
  using Callback = std::function<void(const Result&)>;

  virtual void send(const std::string& message, const Callback& cb) = 0;

 protected:
  Sender() = default;
};

}  // namespace udp
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_UDP_SENDER_H_
