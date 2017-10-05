#ifndef AIRMAP_REST_RULESETS_H_
#define AIRMAP_REST_RULESETS_H_

#include <airmap/client.h>
#include <airmap/rulesets.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

/// RuleSets provides functionality for managing contextual airspace.
class RuleSets : public airmap::RuleSets {
 public:
  explicit RuleSets(Client::Version version, const std::shared_ptr<net::http::Requester>& requester);

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;

 private:
  Client::Version version_;
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_RULESETS_H_
