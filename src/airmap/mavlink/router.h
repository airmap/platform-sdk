//
//  router.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MAVLINK_ROUTER_H_
#define AIRMAP_MAVLINK_ROUTER_H_

#include <airmap/do_not_copy_or_move.h>

#include <standard/mavlink.h>

#include <initializer_list>
#include <memory>
#include <vector>

namespace airmap {
namespace mavlink {

/// Router abstracts routing of incoming mavlink messages.
class Router {
 public:
  /// Route abstracts an outgoing route for mavlink messages.
  class Route : DoNotCopyOrMove {
   public:
    /// process is invoked for every incoming message 'message'.
    virtual void process(const mavlink_message_t& message) = 0;

    /// start spins up the Route instance.
    virtual void start() = 0;

    /// stop stops the Route instance.
    virtual void stop() = 0;

   protected:
    Route() = default;
  };

  /// Router initializes a new instance with 'routes'
  explicit Router(const std::initializer_list<std::shared_ptr<Route>>& routes);

  /// route hands 'message' to all known routes.
  void route(const mavlink_message_t& message);

  /// start spins up the router and all configured routes.
  void start();

  /// stop stops the router and all configured routes.
  void stop();

 private:
  std::vector<std::shared_ptr<Route>> routes_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_ROUTER_H_
