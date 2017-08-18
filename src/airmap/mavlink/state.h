#ifndef AIRMAP_MAVLINK_STATE_H_
#define AIRMAP_MAVLINK_STATE_H_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#include <standard/mavlink.h>
#pragma clang diagnostic pop

#include <iosfwd>

namespace airmap {
namespace mavlink {

using State = MAV_STATE;

}  // namespace mavlink
}  // namespace airmap

std::ostream& operator<<(std::ostream& out, airmap::mavlink::State state);

#endif  // AIRMAP_MAVLINK_STATE_H_