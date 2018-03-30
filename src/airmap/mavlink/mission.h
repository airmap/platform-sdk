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
  airmap::Geometry get_plan_geometry();

 private:
  void handle_msg_mission_count(const mavlink_message_t& msg);
  bool handle_msg_mission_item(const mavlink_message_t& msg);

  uint16_t max_count_  = 0;
  uint16_t curr_count_ = 0;

  std::vector<Geometry::Coordinate> coordinates_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_MISSION_H_
