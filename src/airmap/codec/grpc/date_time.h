//
//  date_time.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_GRPC_DATE_TIME_H_
#define AIRMAP_CODEC_GRPC_DATE_TIME_H_

#include <airmap/date_time.h>

#include <google/protobuf/timestamp.pb.h>

namespace airmap {
namespace codec {
namespace grpc {

void decode(const ::google::protobuf::Timestamp& from, DateTime& to);
void encode(::google::protobuf::Timestamp& to, const DateTime& from);

}  // namespace grpc
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_GRPC_DATE_TIME_H_