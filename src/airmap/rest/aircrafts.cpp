#include <airmap/rest/aircrafts.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <fmt/printf.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string airmap::rest::Aircrafts::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/aircraft/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v2");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Aircrafts::Aircrafts(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Aircrafts::manufacturers(const Manufacturers::Parameters& parameters,
                                            const Manufacturers::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/manufacturer", std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::parse_to_outcome<std::vector<Aircraft::Manufacturer>>(result.value().body));
                    } else {
                      cb(Manufacturers::Result{result.error()});
                    }
                  });
}

void airmap::rest::Aircrafts::models(const Models::Parameters& parameters, const Models::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/model", std::move(query), std::move(headers), [cb](const net::http::Requester::Result& result) {
    if (result) {
      cb(jsend::parse_to_outcome<std::vector<Aircraft>>(result.value().body));
    } else {
      cb(Models::Result{result.error()});
    }
  });
}

void airmap::rest::Aircrafts::model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  requester_->get(fmt::sprintf("/model/%s", parameters.id), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::parse_to_outcome<Aircraft>(result.value().body));
                    } else {
                      cb(ModelForId::Result{result.error()});
                    }
                  });
}
