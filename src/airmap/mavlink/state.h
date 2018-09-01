//
//  state.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MAVLINK_STATE_H_
#define AIRMAP_MAVLINK_STATE_H_

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

#include <iosfwd>

namespace airmap {
namespace mavlink {

using State = MAV_STATE;

}  // namespace mavlink
}  // namespace airmap

std::ostream& operator<<(std::ostream& out, airmap::mavlink::State state);

#endif  // AIRMAP_MAVLINK_STATE_H_