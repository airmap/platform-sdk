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
#ifndef AIRMAP_REST_FLIGHT_PLANS_H_
#define AIRMAP_REST_FLIGHT_PLANS_H_

#include <airmap/client.h>
#include <airmap/flight_plans.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

/// FlightPlans provides functionality for managing flight plans.
class FlightPlans : public airmap::FlightPlans {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit FlightPlans(const std::shared_ptr<net::http::Requester>& requester);

  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;
  void create_by_polygon(const Create::Parameters& parameters, const Create::Callback& cb) override;
  void update(const Update::Parameters& parameters, const Update::Callback& cb) override;
  void delete_(const Delete::Parameters& parameters, const Delete::Callback& cb) override;
  void render_briefing(const RenderBriefing::Parameters& parameters, const RenderBriefing::Callback& cb) override;
  void submit(const Submit::Parameters& parameters, const Submit::Callback& cb) override;

  void set_auth_token(std::string token);

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_FLIGHT_PLANS_H_
