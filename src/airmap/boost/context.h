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
  void create_monitor_client_with_configuration(const monitor::Client::Configuration& configuration,
                                                const MonitorClientCreateCallback& cb) override;

  ReturnCode exec(const SignalSet& signal_set, const SignalHandler& signal_handler) override;
  ReturnCode run() override;
  void stop(ReturnCode rc) override;
  void schedule_in(const Microseconds& wait_for, const std::function<void()>& functor) override;
  void dispatch(const std::function<void()>& task) override;

 private:
  enum class State { stopped, stopping, running };
  explicit Context(const std::shared_ptr<Logger>& logger);

  std::shared_ptr<net::http::Requester> advisory(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> aircrafts(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> airspaces(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> authenticator(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> elevation(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> flights(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> flight_plans(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> pilots(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> rulesets(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> status(const airmap::Client::Configuration& configuration);
  std::shared_ptr<net::http::Requester> sso(const airmap::Client::Configuration& configuration);

  util::FormattingLogger log_;

  std::shared_ptr<::boost::asio::io_service> io_service_;
  std::shared_ptr<::boost::asio::io_service::work> keep_alive_;
  std::atomic<State> state_;
  std::atomic<ReturnCode> return_code_;
};

}  // namespace boost
}  // namespace airmap

#endif  // AIRMAP_BOOST_CONTEXT_H_
