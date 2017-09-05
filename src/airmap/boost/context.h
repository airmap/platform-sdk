#ifndef AIRMAP_BOOST_CONTEXT_H_
#define AIRMAP_BOOST_CONTEXT_H_

#include <airmap/context.h>
#include <airmap/rest/client.h>
#include <airmap/util/formatting_logger.h>

#include <boost/asio.hpp>

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace airmap {
namespace boost {

class Context : public airmap::Context, public std::enable_shared_from_this<Context> {
 public:
  static std::shared_ptr<Context> create(const std::shared_ptr<Logger>& logger);

  ~Context();

  // Enable access to the underlying io_service such that other elements of
  // the component can integrate easily.
  const std::shared_ptr<::boost::asio::io_service>& io_service() const;

  // From airmap::Context
  void create_client_with_configuration(const Client::Configuration& configuration,
                                        const ClientCreateCallback& cb) override;

  ReturnCode exec(const SignalSet& signal_set, const SignalHandler& signal_handler) override;
  ReturnCode run() override;
  void stop(ReturnCode rc) override;

  void dispatch(const std::function<void()>& task);

 private:
  enum class State { stopped, stopping, running };
  explicit Context(const std::shared_ptr<Logger>& logger);

  util::FormattingLogger log_;

  std::shared_ptr<::boost::asio::io_service> io_service_;
  std::shared_ptr<::boost::asio::io_service::work> keep_alive_;
  std::atomic<State> state_;
  std::atomic<ReturnCode> return_code_;
};

}  // namespace boost
}  // namespace airmap

#endif  // AIRMAP_BOOST_CONTEXT_H_
