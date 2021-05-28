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
#include <airmap/rest/rulesets.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/net/http/authorized_requester.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::RuleSets::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/rules/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::RuleSets::RuleSets(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::RuleSets::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  json j = parameters;

  requester_->post("/", std::move(headers), j.dump(),
                   net::http::jsend_parsing_request_callback<std::vector<RuleSet>>(cb));
}

void airmap::rest::RuleSets::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  requester_->get(fmt::sprintf("/%s", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<RuleSet>(cb));
}

void airmap::rest::RuleSets::fetch_rules(const FetchRules::Parameters& parameters, const FetchRules::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/rule", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<std::vector<RuleSet>>(cb));
}

void airmap::rest::RuleSets::evaluate_rulesets(const EvaluateRules::Parameters& parameters,
                                               const EvaluateRules::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  json j = parameters;

  requester_->post("/evaluation", std::move(headers), j.dump(),
                   net::http::jsend_parsing_request_callback<Evaluation>(cb));
}

void airmap::rest::RuleSets::evaluate_flight_plan(const EvaluateFlightPlan::Parameters& parameters,
                                                  const EvaluateFlightPlan::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  requester_->get(fmt::sprintf("/%s/evaluation", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Evaluation>(cb));
}

void airmap::rest::RuleSets::set_auth_token(std::string token) {
  airmap::net::http::AuthorizedRequester *auth_requester = dynamic_cast<airmap::net::http::AuthorizedRequester*>(requester_.get());
  if (auth_requester) {
    auth_requester->set_auth_token(token);
  }
}
