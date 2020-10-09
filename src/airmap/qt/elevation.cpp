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
#include <airmap/qt/elevation.h>

std::shared_ptr<airmap::qt::Elevation> airmap::qt::Elevation::create(const std::shared_ptr<Dispatcher>& dispatcher,
                                                                     const std::shared_ptr<airmap::Client>& client) {
  return std::shared_ptr<Elevation>{new Elevation{dispatcher, client}};
}

airmap::qt::Elevation::Elevation(const std::shared_ptr<Dispatcher>& dispatcher,
                                 const std::shared_ptr<airmap::Client>& client)
    : dispatcher_{dispatcher}, client_{client} {
}

void airmap::qt::Elevation::get_elevation_points(const GetElevationPoints::Parameters& parameters,
                                                 const GetElevationPoints::Callback& cb) {
  dispatcher_->dispatch_to_native([this, sp = shared_from_this(), parameters, cb]() {
    sp->client_->elevation().get_elevation_points(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::Elevation::get_elevation_carpet(const GetElevationCarpet::Parameters& parameters,
                                                 const GetElevationCarpet::Callback& cb) {
  dispatcher_->dispatch_to_native([this, sp = shared_from_this(), parameters, cb]() {
    sp->client_->elevation().get_elevation_carpet(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}

void airmap::qt::Elevation::get_elevation_points(const GetElevationPath::Parameters& parameters,
                                                 const GetElevationPath::Callback& cb) {
  dispatcher_->dispatch_to_native([this, sp = shared_from_this(), parameters, cb]() {
    sp->client_->elevation().get_elevation_path(parameters, [this, sp, cb](const auto& result) {
      sp->dispatcher_->dispatch_to_qt([sp, result, cb]() { cb(result); });
    });
  });
}
