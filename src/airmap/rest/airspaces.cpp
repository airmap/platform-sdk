#include <airmap/rest/airspaces.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <fmt/printf.h>

#include <sstream>

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

airmap::rest::Airspaces::Airspaces(Client::Version version, const std::shared_ptr<net::http::Requester>& requester)
    : version_{version}, requester_{requester} {
}

void airmap::rest::Airspaces::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/airspace/%s/search"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<Airspace>>(json::parse(result.value().body)));
                    } else {
                      cb(Search::Result{result.error()});
                    }
                  });
}

void airmap::rest::Airspaces::for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  requester_->get(version_to_path(version_, "/airspace/%s/%s", parameters.id), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<Airspace>>(json::parse(result.value().body)));
                    } else {
                      cb(ForIds::Result{result.error()});
                    }
                  });
}
