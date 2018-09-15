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
//  traffic.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CODEC_GRPC_TRAFFIC_H_
#define AIRMAP_CODEC_GRPC_TRAFFIC_H_

#include <airmap/traffic.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <grpc/airmap/monitor/monitor.grpc.pb.h>
#pragma GCC diagnostic push

namespace airmap {
namespace codec {
namespace grpc {

void decode(const ::grpc::airmap::Traffic_Update& from, Traffic::Update& to);
void encode(::grpc::airmap::Traffic_Update& to, const Traffic::Update& from);

}  // namespace grpc
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_GRPC_TRAFFIC_H_