//
//  date_time.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/grpc/date_time.h>

void airmap::codec::grpc::decode(const ::google::protobuf::Timestamp& from, DateTime& to) {
  auto us = (from.seconds() * 1000 * 1000) + (from.nanos() / 1000);
  to      = from_microseconds_since_epoch(microseconds(us));
}

void airmap::codec::grpc::encode(::google::protobuf::Timestamp& to, const DateTime& from) {
  auto us = microseconds_since_epoch(from);
  auto s  = us / (1000 * 1000);

  to.set_seconds(s);
  to.set_nanos(1000 * (us - (s * 1000 * 1000)));
}
