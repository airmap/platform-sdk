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
//  method_invocation.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_GRPC_METHOD_INVICATION_H_
#define AIRMAP_GRPC_METHOD_INVICATION_H_

#include <airmap/do_not_copy_or_move.h>

#include <iosfwd>

namespace airmap {
namespace grpc {

/// MethodInvocation abstracts the state of an individual method invocation.
class MethodInvocation : DoNotCopyOrMove {
 public:
  // State enumerates all known states of a MethodInvocation instance.
  enum class State { ready = 0, streaming = 1, finished = 2 };

  /// proceed is invoked on every MethodInvocation instance
  /// dequeued by an Executor. If 'result' is true, the previous
  /// operation was successful, false otherwise.
  virtual void proceed(bool result) = 0;

 protected:
  MethodInvocation() = default;
};

/// operator<< inserts 'state' into 'out'.
std::ostream& operator<<(std::ostream& out, MethodInvocation::State state);

}  // namespace grpc
}  // namespace airmap

#endif  // AIRMAP_GRPC_METHOD_INVICATION_H_