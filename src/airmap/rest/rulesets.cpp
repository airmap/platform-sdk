#include <airmap/rest/rulesets.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>

#include <fmt/printf.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
