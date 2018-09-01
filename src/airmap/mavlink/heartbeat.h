//
//  heartbeat.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_MAVLINK_HEARTBEAT_H_
#define AIRMAP_MAVLINK_HEARTBEAT_H_

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

using Heartbeat = mavlink_heartbeat_t;

}  // namespace mavlink
}  // namespace airmap

std::ostream& operator<<(std::ostream& out, const airmap::mavlink::Heartbeat& heartbeat);

#endif  // AIRMAP_MAVLINK_HEARTBEAT_H_