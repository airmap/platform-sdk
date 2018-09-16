/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  channel.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MAVLINK_CHANNEL_H_
#define AIRMAP_MAVLINK_CHANNEL_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif
#include <standard/mavlink.h>
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <vector>

namespace airmap {
namespace mavlink {

class Channel : DoNotCopyOrMove {
 public:
  struct Counters {
    std::uint64_t received{0};
    std::uint64_t good{0};
    std::uint64_t bad{0};
  };

  using Subscriber    = std::function<void(const mavlink_message_t&)>;
  using SubscriberSet = std::list<Subscriber>;
  using Subscription  = SubscriberSet::const_iterator;

  const Counters& counters() const;

  Subscription subscribe(const Subscriber&);
  void unsubscribe(Subscription&& subscription);

  void start();
  void stop();

 protected:
  Channel();

  virtual void start_impl() = 0;
  virtual void stop_impl()  = 0;

  void invoke_subscribers(const std::vector<mavlink_message_t>& msgs);
  Optional<std::vector<mavlink_message_t>> process_mavlink_data(const char* begin, const char* end);

 private:
  Counters counters_;
  std::mutex guard_;
  SubscriberSet subscribers_;
  struct {
    mavlink_message_t msg;
    mavlink_status_t status;
  } parse_buffer_;
  struct {
    mavlink_message_t msg;
    mavlink_status_t status;
  } parse_out_;
};

class FilteringChannel : public Channel, public std::enable_shared_from_this<FilteringChannel> {
 public:
  static std::shared_ptr<FilteringChannel> create(const std::shared_ptr<Channel>& next, std::uint8_t system_id);
  ~FilteringChannel();

 protected:
  void start_impl();
  void stop_impl();

 private:
  explicit FilteringChannel(const std::shared_ptr<Channel>& next, std::uint8_t system_id);

  std::shared_ptr<Channel> next_;
  std::uint8_t system_id_;
  Subscription subscription_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_CHANNEL_H_