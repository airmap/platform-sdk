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
//  mission.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MAVLINK_MISSION_H_
#define AIRMAP_MAVLINK_MISSION_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>
#include <airmap/util/formatting_logger.h>

#include <airmap/geometry.h>
#include <airmap/mavlink/state.h>
#include <cstdint>

#include <memory>
#include <unordered_set>

namespace airmap {
namespace mavlink {

class Mission {
 public:
  bool update(const mavlink_message_t& msg);
  const std::vector<Geometry::Coordinate>& coordinates() const;

 private:
  void handle_msg_mission_clear_all();
  void handle_msg_mission_count(const mavlink_message_t& msg);
  bool handle_msg_mission_item(const mavlink_message_t& msg);

  std::vector<Geometry::Coordinate> coordinates_;
  std::size_t counter_      = 0;
  std::size_t mission_size_ = 0;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_MISSION_H_
