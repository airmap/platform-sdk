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
  coordinates_.clear();
  counter_      = 0;
  mission_size_ = 0;
}

void airmap::mavlink::Mission::handle_msg_mission_count(const mavlink_message_t& msg) {
  mavlink_mission_count_t mc;
  mavlink_msg_mission_count_decode(&msg, &mc);

  coordinates_.clear();
  counter_      = 0;
  mission_size_ = mc.count;
}

bool airmap::mavlink::Mission::handle_msg_mission_item(const mavlink_message_t& msg) {
  mavlink_mission_item_t mi;
  mavlink_msg_mission_item_decode(&msg, &mi);

  // We have been expecting this element and start processing.
  if (mi.seq == counter_) {
    counter_++;

    // This is relevant to our mission and we store the waypoint in memory.
    if (mi.mission_type == MAV_CMD_NAV_WAYPOINT) {
      coordinates_.push_back({mi.x, mi.y, mi.z});
    }

    // Is the mission complete, yet?
    if (counter_ == mission_size_) {
      return true;
    }
  }

  return false;
}
