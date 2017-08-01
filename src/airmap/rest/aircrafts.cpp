#include <airmap/rest/aircrafts.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <boost/format.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

airmap::rest::Aircrafts::Aircrafts(Communicator& communicator) : communicator_{communicator} {
}

void airmap::rest::Aircrafts::manufacturers(const Manufacturers::Parameters& parameters,
                                            const Manufacturers::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  communicator_.get("api.airmap.com", "/aircraft/v2/manufacturer", std::move(query), std::move(headers),
                    [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Aircraft::Manufacturer>>(json::parse(result.value())));
                      }
                    });
}

void airmap::rest::Aircrafts::models(const Models::Parameters& parameters, const Models::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  communicator_.get("api.airmap.com", "/aircraft/v2/model", std::move(query), std::move(headers),
                    [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Aircraft>>(json::parse(result.value())));
                      }
                    });
}

void airmap::rest::Aircrafts::model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  communicator_.get("api.airmap.com", (boost::format("/aircraft/v2/model/%1%") % parameters.id).str(), std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<Aircraft>(json::parse(result.value())));
                      }
                    });
}