#include <airmap/rest/aircrafts.h>

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
      return fmt::sprintf(pattern, "v2", std::forward<Args>(args)...);
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage", std::forward<Args>(args)...);
  }

  throw std::logic_error{"should not reach here"};
}

}  // namespace

airmap::rest::Aircrafts::Aircrafts(Client::Version version, const std::shared_ptr<net::http::Requester>& requester)
    : version_{version}, requester_{requester} {
}

void airmap::rest::Aircrafts::manufacturers(const Manufacturers::Parameters& parameters,
                                            const Manufacturers::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/aircraft/%s/manufacturer"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<Aircraft::Manufacturer>>(json::parse(result.value().body)));
                    } else {
                      cb(Manufacturers::Result{result.error()});
                    }
                  });
}

void airmap::rest::Aircrafts::models(const Models::Parameters& parameters, const Models::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/aircraft/%s/model"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<Aircraft>>(json::parse(result.value().body)));
                    } else {
                      cb(Models::Result{result.error()});
                    }
                  });
}

void airmap::rest::Aircrafts::model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  requester_->get(version_to_path(version_, "/aircraft/%s/model/%s", parameters.id), std::move(query),
                  std::move(headers), [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<Aircraft>(json::parse(result.value().body)));
                    } else {
                      cb(ModelForId::Result{result.error()});
                    }
                  });
}
