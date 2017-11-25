#ifndef AIRMAP_CODEC_GRPC_TRAFFIC_H_
#define AIRMAP_CODEC_GRPC_TRAFFIC_H_

#include <airmap/traffic.h>

#include "grpc/airmap/monitor/monitor.grpc.pb.h"

namespace airmap {
namespace codec {
namespace grpc {

void decode(const ::grpc::airmap::Traffic_Update& from, Traffic::Update& to);
void encode(::grpc::airmap::Traffic_Update& to, const Traffic::Update& from);

}  // namespace grpc
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_GRPC_TRAFFIC_H_