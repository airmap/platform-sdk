#ifndef AIRMAP_MAVLINK_GLOBAL_POSITION_INT_H_
#define AIRMAP_MAVLINK_GLOBAL_POSITION_INT_H_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#include <standard/mavlink.h>
#pragma clang diagnostic pop

#include <iosfwd>

namespace airmap {
namespace mavlink {

using GlobalPositionInt = mavlink_global_position_int_t;

}  // namespace mavlink
}  // namespace airmap

std::ostream& operator<<(std::ostream& out, const airmap::mavlink::GlobalPositionInt& position);

#endif  // AIRMAP_MAVLINK_GLOBAL_POSITION_INT_H_