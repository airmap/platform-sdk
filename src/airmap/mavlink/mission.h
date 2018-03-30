#ifndef AIRMAP_MAVLINK_MISSION_H_
#define AIRMAP_MAVLINK_MISSION_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>
#include <airmap/util/formatting_logger.h>

#include <airmap/geometry.h>
#include <airmap/mavlink/vehicle.h>
#include <airmap/mavlink/state.h>

#include <cstdint>

#include <memory>
#include <unordered_set>

namespace airmap {
namespace mavlink {

class Mission {
 public:
  explicit Mission(std::shared_ptr<Vehicle> v);
  void update(const mavlink_message_t& msg);

 protected:
  void handle_msg_mission_count(const mavlink_message_t& msg);
  void handle_msg_mission_item(const mavlink_message_t& msg);
  void send_msg_mission_request();
  void send_msg_mission_ack();

  void build_mission();

  uint16_t max_count_ = 0;
  uint16_t curr_count_ = 0;

  std::vector<Geometry::Coordinate> coordinates_;

  std::shared_ptr<Vehicle> vehicle_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_MISSION_H_
