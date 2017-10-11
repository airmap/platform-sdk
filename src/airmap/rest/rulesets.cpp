#include <airmap/rest/rulesets.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <fmt/printf.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

template <typename... Args>
std::string version_to_path(airmap::Client::Version version, const char* pattern, Args&&... args) {
  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1", std::forward<Args>(args)...);
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage", std::forward<Args>(args)...);
  }

  throw std::logic_error{"should not reach here"};
}

}  // namespace

airmap::rest::RuleSets::RuleSets(Client::Version version, const std::shared_ptr<net::http::Requester>& requester)
    : version_{version}, requester_{requester} {
}

void airmap::rest::RuleSets::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/rules/%s"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<RuleSet>>(json::parse(result.value().body)));
                    } else {
                      cb(Search::Result{result.error()});
                    }
                  });
}

void airmap::rest::RuleSets::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  requester_->get(version_to_path(version_, "/rules/%s/%s", parameters.id), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<RuleSet>(json::parse(result.value().body)));
                    } else {
                      cb(ForId::Result{result.error()});
                    }
                  });
}

void airmap::rest::RuleSets::fetch_rules(const FetchRules::Parameters& parameters, const FetchRules::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/rules/%s/rule"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<RuleSet>>(json::parse(result.value().body)));
                    } else {
                      cb(FetchRules::Result{result.error()});
                    }
                  });
}

void airmap::rest::RuleSets::evaluate_rulesets(const Evaluation::Parameters& parameters, const Evaluation::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/rules/%s/evaluation"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<RuleSet>(json::parse(result.value().body)));
                    } else {
                      cb(Evaluation::Result{result.error()});
                    }
                  });
}