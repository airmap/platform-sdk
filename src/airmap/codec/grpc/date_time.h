/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  date_time.h
//  AirMap Platform SDK
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