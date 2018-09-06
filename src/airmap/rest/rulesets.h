//
//  rulesets.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_REST_RULESETS_H_
#define AIRMAP_REST_RULESETS_H_

#include <airmap/client.h>
#include <airmap/evaluation.h>
#include <airmap/net/http/requester.h>
#include <airmap/rulesets.h>

namespace airmap {
namespace rest {

class RuleSets : public airmap::RuleSets {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit RuleSets(const std::shared_ptr<net::http::Requester>& requester);

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;
  void fetch_rules(const FetchRules::Parameters& parameters, const FetchRules::Callback& cb) override;
  void evaluate_rulesets(const EvaluateRules::Parameters& parameters, const EvaluateRules::Callback& cb) override;

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_RULESETS_H_
