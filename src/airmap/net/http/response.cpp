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
//  response.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/net/http/response.h>

// classify returns the Classification of the status of this response.
airmap::net::http::Response::Classification airmap::net::http::Response::classify() const {
  if (100 <= status && status < 200) {
    return Classification::informational;
  } else if (200 <= status && status < 300) {
    return Classification::success;
  } else if (300 <= status && status < 400) {
    return Classification::redirection;
  } else if (400 <= status && status < 500) {
    return Classification::client_error;
  } else if (500 <= status && status < 600) {
    return Classification::server_error;
  }

  return Classification::unclassified;
}