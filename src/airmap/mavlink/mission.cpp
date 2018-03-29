#include <airmap/mavlink/mission.h>

void airmap::mavlink::Mission::update(const mavlink_message_t& msg) {
  switch (msg.msgid) {
    case MAVLINK_MSG_ID_MISSION_COUNT:
      handle_msg_mission_count(msg);
      break;
    case MAVLINK_MSG_ID_MISSION_ITEM:
      handle_msg_mission_item(msg);
      break;
  }

  build_mission();
}

void airmap::mavlink::Mission::handle_msg_mission_count(const mavlink_message_t& msg) {
  	mavlink_mission_count_t mc;
  	mavlink_msg_mission_count_decode(&msg, &mc);
	max_count_ = mc.count;
	curr_count_ = 0;
	mission_items_.clear();
}

void airmap::mavlink::Mission::handle_msg_mission_item(const mavlink_message_t& msg) {
	mavlink_mission_item_t mi;
  	mavlink_msg_mission_item_decode(&msg, &mi);
  	if (mi.seq == curr_count_) {
		curr_count_++;
		mission_items_.push_back(mi);
  	}
}

void airmap::mavlink::Mission::send_msg_mission_request() {

}

void airmap::mavlink::Mission::send_msg_mission_ack() {

}

void airmap::mavlink::Mission::build_mission() {
	if (curr_count_ < max_count_) {
		send_msg_mission_request();
	} else if (curr_count_ == max_count_) {
		send_msg_mission_ack();
	}
}