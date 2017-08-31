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

airmap::rest::Airspaces::Airspaces(const std::string& host, Client::Version version,
                                   airmap::rest::Communicator& communicator)
    : host_{host}, version_{version}, communicator_{communicator} {
}

void airmap::rest::Airspaces::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  communicator_.get(host_, version_to_path(version_, "/airspace/%s/search"), std::move(query), std::move(headers),
                    [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Airspace>>(json::parse(result.value())));
                      } else {
                        cb(Search::Result{result.error()});
                      }
                    });
}

void airmap::rest::Airspaces::for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  communicator_.get(host_, version_to_path(version_, "/airspace/%s/%s", parameters.id), std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Airspace>>(json::parse(result.value())));
                      } else {
                        cb(ForIds::Result{result.error()});
                      }
                    });
}
