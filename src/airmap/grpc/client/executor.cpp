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
#include <airmap/grpc/client/executor.h>

#include <airmap/grpc/method_invocation.h>

void airmap::grpc::client::Executor::invoke_method(const std::function<void(::grpc::CompletionQueue*)>& f) {
  f(&completion_queue_);
}

void airmap::grpc::client::Executor::run() {
  bool ok   = false;
  void* tag = nullptr;

  while (completion_queue_.Next(&tag, &ok)) {
    if (auto mi = static_cast<MethodInvocation*>(tag)) {
      mi->proceed(ok);
    }
  }
}

void airmap::grpc::client::Executor::stop() {
  completion_queue_.Shutdown();
}
