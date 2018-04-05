#include <airmap/mavlink/mission.h>

bool airmap::mavlink::Mission::update(const mavlink_message_t& msg) {
  switch (msg.msgid) {
    case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:
      handle_msg_mission_clear_all();
      break;
    case MAVLINK_MSG_ID_MISSION_COUNT:
      handle_msg_mission_count(msg);
      break;
    case MAVLINK_MSG_ID_MISSION_ITEM:
      return handle_msg_mission_item(msg);
  }
  return false;
}

const std::vector<airmap::Geometry::Coordinate>& airmap::mavlink::Mission::coordinates() const {
  return coordinates_;
}

void airmap::mavlink::Mission::handle_msg_mission_clear_all() {
  max_count_  = 0;
  curr_count_ = 0;
  coordinates_.clear();
}

void airmap::mavlink::Mission::handle_msg_mission_count(const mavlink_message_t& msg) {
  mavlink_mission_count_t mc;
  mavlink_msg_mission_count_decode(&msg, &mc);
  max_count_  = mc.count;
  curr_count_ = 0;
  coordinates_.clear();
}

bool airmap::mavlink::Mission::handle_msg_mission_item(const mavlink_message_t& msg) {
  mavlink_mission_item_t mi;
  mavlink_msg_mission_item_decode(&msg, &mi);
  if (mi.seq == curr_count_) {
    curr_count_++;
    if (mi.mission_type == MAV_CMD_NAV_WAYPOINT) {
      Geometry::Coordinate c = {mi.x, mi.y};
      coordinates_.push_back(c);
      if (curr_count_ == max_count_) {
        return true;
      }
    }
  }
  return false;
}
