// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
