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
//  do_not_copy_or_move.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_DO_NOT_COPY_OR_MOVE_H_
#define AIRMAP_DO_NOT_COPY_OR_MOVE_H_

#include <airmap/visibility.h>

namespace airmap {

/// @cond
struct AIRMAP_EXPORT DoNotCopyOrMove {
  DoNotCopyOrMove()                       = default;
  DoNotCopyOrMove(const DoNotCopyOrMove&) = delete;
  DoNotCopyOrMove(DoNotCopyOrMove&&)      = delete;
  virtual ~DoNotCopyOrMove()              = default;
  DoNotCopyOrMove& operator=(const DoNotCopyOrMove&) = delete;
  DoNotCopyOrMove& operator=(DoNotCopyOrMove&&) = delete;
};
/// @endcond

}  // namespace airmap

#endif  // AIRMAP_DO_NOT_COPY_OR_MOVE_H_
