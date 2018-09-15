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
//  request.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_NET_HTTP_REQUEST_H_
#define AIRMAP_NET_HTTP_REQUEST_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/error.h>
#include <airmap/net/http/response.h>
#include <airmap/outcome.h>

#include <functional>
#include <stdexcept>
#include <string>

namespace airmap {
namespace net {
namespace http {

class Request : DoNotCopyOrMove {
 public:
  using Result   = Outcome<Response, Error>;
  using Callback = std::function<void(const Result&)>;

  virtual void execute(const Callback& cb) = 0;

 protected:
  Request() = default;
};

}  // namespace http
}  // namespace net
}  // namespace airmap

#endif  // AIRMAP_NET_HTTP_REQUEST_H_
