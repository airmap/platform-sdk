#include <airmap/rest/airspaces.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <boost/format.hpp>

#include <sstream>

using json = nlohmann::json;

airmap::rest::Airspaces::Airspaces(airmap::rest::Communicator& communicator) : communicator_{communicator} {
}

void airmap::rest::Airspaces::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  communicator_.get("api.airmap.com", "/airspace/v2/search", std::move(query), std::move(headers),
                    [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Airspace>>(json::parse(result.value())));
                      }
                    });
}

void airmap::rest::Airspaces::for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  communicator_.get("api.airmap.com", (boost::format("/airspace/v2/%1%") % parameters.id).str(), std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Airspace>>(json::parse(result.value())));
                      }
                    });
}
