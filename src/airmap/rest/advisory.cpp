#include <airmap/rest/advisory.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <boost/format.hpp>
#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Advisory::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/advisory/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Advisory::Advisory(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Advisory::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(fmt::sprintf("/airspace/%s", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<std::vector<AirspaceAdvisory>>(cb));
}

void airmap::rest::Advisory::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j = parameters;

  requester_->post("/airspace", std::move(headers), j.dump(),
                   net::http::jsend_parsing_request_callback<std::vector<AirspaceAdvisory>>(cb));
}

void airmap::rest::Advisory::report_weather(const ReportWeather::Parameters& parameters,
                                            const ReportWeather::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/weather", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Weather>(cb));
}
