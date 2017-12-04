#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE rest

#include <airmap/net/http/requester.h>

#include <airmap/rest/aircrafts.h>
#include <airmap/rest/airspaces.h>
#include <airmap/rest/flights.h>
#include <airmap/rest/pilots.h>

#include <airmap/codec.h>

#include <boost/test/unit_test.hpp>
#include <trompeloeil/trompeloeil.hpp>

namespace mock = trompeloeil;

namespace {

using StringMap = std::unordered_map<std::string, std::string>;

struct MockHttpRequester : public airmap::net::http::Requester {
  MAKE_MOCK4(delete_, void(const std::string&, StringMap&&, StringMap&&, Callback), override);
  MAKE_MOCK4(get, void(const std::string&, StringMap&&, StringMap&&, Callback), override);
  MAKE_MOCK4(patch, void(const std::string&, StringMap&&, const std::string&, Callback), override);
  MAKE_MOCK4(post, void(const std::string&, StringMap&&, const std::string&, Callback), override);
};

constexpr const char* host{"api.airmap.test.com"};
constexpr airmap::Client::Version version{airmap::Client::Version::production};

}  // namespace

using mock::_;

BOOST_AUTO_TEST_CASE(api_aircraft_search_for_manufactureres_issues_get_request_with_correct_parameters) {
  airmap::Aircrafts::Manufacturers::Parameters parameters;
  StringMap query;
  airmap::codec::http::query::encode(query, parameters);

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/aircraft/v2/manufacturer"), mock::eq<StringMap>(query),
                               ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Aircrafts aircrafts{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Aircrafts::default_route_for_version(version), requester)};
  aircrafts.manufacturers(parameters, [](const airmap::Aircrafts::Manufacturers::Result&) {});
}

BOOST_AUTO_TEST_CASE(api_aircraft_search_for_models_issues_get_request_with_correct_parameters) {
  airmap::Aircrafts::Models::Parameters parameters;
  StringMap query;
  airmap::codec::http::query::encode(query, parameters);

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/aircraft/v2/model"), mock::eq<StringMap>(query), ANY(StringMap),
                               ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Aircrafts aircrafts{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Aircrafts::default_route_for_version(version), requester)};
  aircrafts.models(parameters, [](const airmap::Aircrafts::Models::Result&) {});
}

BOOST_AUTO_TEST_CASE(api_aircraft_search_for_model_by_id_issues_get_request_with_correct_parameters) {
  airmap::Aircrafts::ModelForId::Parameters parameters;
  parameters.id = "42";

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/aircraft/v2/model/" + parameters.id), ANY(StringMap),
                               ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Aircrafts aircrafts{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Aircrafts::default_route_for_version(version), requester)};
  aircrafts.model_for_id(parameters, [](const airmap::Aircrafts::ModelForId::Result&) {});
}

BOOST_AUTO_TEST_CASE(api_airspace_search_issues_get_request_with_correct_parameters) {
  airmap::Airspaces::Search::Parameters parameters;
  StringMap query;
  airmap::codec::http::query::encode(query, parameters);

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/airspace/v2/search"), mock::eq<StringMap>(query), ANY(StringMap),
                               ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Airspaces airspaces{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Airspaces::default_route_for_version(version), requester)};
  airspaces.search(parameters, [](const airmap::Airspaces::Search::Result&) {});
}
BOOST_AUTO_TEST_CASE(api_airspace_for_id_issues_get_request_with_correct_parameters) {
  airmap::Airspaces::ForIds::Parameters parameters;
  parameters.id = "42";

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/airspace/v2/" + parameters.id), ANY(StringMap), ANY(StringMap),
                               ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Airspaces airspaces{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Airspaces::default_route_for_version(version), requester)};
  airspaces.for_ids(parameters, [](const airmap::Airspaces::ForIds::Result&) {});
}

BOOST_AUTO_TEST_CASE(api_flights_search_issues_get_request_with_correct_parameters) {
  airmap::Flights::Search::Parameters parameters;
  StringMap query;
  airmap::codec::http::query::encode(query, parameters);

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/flight/v2/"), mock::eq<StringMap>(query), ANY(StringMap),
                               ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Flights flights{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Flights::default_route_for_version(version), requester)};
  flights.search(parameters, [](const airmap::Flights::Search::Result&) {});
}

BOOST_AUTO_TEST_CASE(api_flights_for_id_issues_get_request_with_correct_parameters) {
  airmap::Flights::ForId::Parameters parameters;
  parameters.id = "flight|abc";

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/flight/v2/" + parameters.id), ANY(StringMap), ANY(StringMap),
                               ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Flights flights{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Flights::default_route_for_version(version), requester)};
  flights.for_id(parameters, [](const airmap::Flights::ForId::Result&) {});
}

BOOST_AUTO_TEST_CASE(api_pilots_authenticated_issues_get_request_with_correct_parameters) {
  airmap::Pilots::Authenticated::Parameters parameters;

  StringMap query;
  airmap::codec::http::query::encode(query, parameters);

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/pilot/v2/profile"), mock::eq<StringMap>(query), ANY(StringMap),
                               ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Pilots pilots{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Pilots::default_route_for_version(version), requester)};
  pilots.authenticated(parameters, [](const auto&) {});
}

BOOST_AUTO_TEST_CASE(api_pilots_for_id_issues_get_request_with_correct_parameters) {
  airmap::Pilots::ForId::Parameters parameters;
  parameters.id = "test";
  StringMap query;
  airmap::codec::http::query::encode(query, parameters);

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/pilot/v2/test"), mock::eq<StringMap>(query), ANY(StringMap),
                               ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Pilots pilots{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Pilots::default_route_for_version(version), requester)};
  pilots.for_id(parameters, [](const auto&) {});
}

BOOST_AUTO_TEST_CASE(api_pilot_aircrafts_issues_get_request_with_correct_parameters) {
  airmap::Pilots::Aircrafts::Parameters parameters;
  parameters.id = "test";
  StringMap query;

  auto requester = std::make_shared<MockHttpRequester>();
  REQUIRE_CALL(*requester, get(mock::eq<std::string>("/pilot/v2/test/aircraft"), mock::eq<StringMap>(query),
                               ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

  airmap::rest::Pilots pilots{std::make_shared<airmap::net::http::RoutingRequester>(
      airmap::rest::Pilots::default_route_for_version(version), requester)};
  pilots.aircrafts(parameters, [](const auto&) {});
}
