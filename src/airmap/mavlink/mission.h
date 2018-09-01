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
