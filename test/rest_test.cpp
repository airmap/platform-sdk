#define CATCH_CONFIG_MAIN

#include <airmap/net/http/requester.h>

#include <airmap/rest/aircrafts.h>
#include <airmap/rest/airspaces.h>
#include <airmap/rest/flights.h>
#include <airmap/rest/pilots.h>

#include <airmap/codec.h>

#include <catch/catch.hpp>
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

TEST_CASE("rest") {
  using mock::_;

  SECTION("aircrafts") {
    SECTION("search for manufactureres issues get request with correct parameters") {
      airmap::Aircrafts::Manufacturers::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/aircraft/v2/manufacturer"), mock::eq<StringMap>(query),
                                   ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Aircrafts aircrafts{version, requester};
      aircrafts.manufacturers(parameters, [](const airmap::Aircrafts::Manufacturers::Result&) {});
    }

    SECTION("search for models issues get request with correct parameters") {
      airmap::Aircrafts::Models::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/aircraft/v2/model"), mock::eq<StringMap>(query),
                                   ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Aircrafts aircrafts{version, requester};
      aircrafts.models(parameters, [](const airmap::Aircrafts::Models::Result&) {});
    }

    SECTION("search for model by id issues get request with correct parameters") {
      airmap::Aircrafts::ModelForId::Parameters parameters;
      parameters.id = "42";

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/aircraft/v2/model/" + parameters.id), ANY(StringMap),
                                   ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Aircrafts aircrafts{version, requester};
      aircrafts.model_for_id(parameters, [](const airmap::Aircrafts::ModelForId::Result&) {});
    }
  }

  SECTION("airspaces") {
    SECTION("search issues get request with correct parameters") {
      airmap::Airspaces::Search::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/airspace/v2/search"), mock::eq<StringMap>(query),
                                   ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Airspaces airspaces{version, requester};
      airspaces.search(parameters, [](const airmap::Airspaces::Search::Result&) {});
    }
    SECTION("for id issues get request with correct parameters") {
      airmap::Airspaces::ForIds::Parameters parameters;
      parameters.id = "42";

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/airspace/v2/" + parameters.id), ANY(StringMap),
                                   ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Airspaces airspaces{version, requester};
      airspaces.for_ids(parameters, [](const airmap::Airspaces::ForIds::Result&) {});
    }
  }

  SECTION("flights") {
    SECTION("search issues get request with correct parameters") {
      airmap::Flights::Search::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/flight/v2"), mock::eq<StringMap>(query), ANY(StringMap),
                                   ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Flights flights{version, requester};
      flights.search(parameters, [](const airmap::Flights::Search::Result&) {});
    }
    SECTION("for id issues get request with correct parameters") {
      airmap::Flights::ForId::Parameters parameters;
      parameters.id = "flight|abc";

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/flight/v2/" + parameters.id), ANY(StringMap), ANY(StringMap),
                                   ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Flights flights{version, requester};
      flights.for_id(parameters, [](const airmap::Flights::ForId::Result&) {});
    }
  }

  SECTION("pilots") {
    SECTION("authenticated issues get request with correct parameters") {
      airmap::Pilots::Authenticated::Parameters parameters;

      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/pilot/v2/profile"), mock::eq<StringMap>(query),
                                   ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Pilots pilots{version, requester};
      pilots.authenticated(parameters, [](const auto&) {});
    }
    SECTION("for_id issues get request with correct parameters") {
      airmap::Pilots::ForId::Parameters parameters;
      parameters.id = "test";
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/pilot/v2/test"), mock::eq<StringMap>(query), ANY(StringMap),
                                   ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Pilots pilots{version, requester};
      pilots.for_id(parameters, [](const auto&) {});
    }
    SECTION("aircrafts issues get request with correct parameters") {
      airmap::Pilots::Aircrafts::Parameters parameters;
      parameters.id = "test";
      StringMap query;

      auto requester = std::make_shared<MockHttpRequester>();
      REQUIRE_CALL(*requester, get(mock::eq<std::string>("/pilot/v2/test/aircraft"), mock::eq<StringMap>(query),
                                   ANY(StringMap), ANY(airmap::net::http::Requester::Callback)));

      airmap::rest::Pilots pilots{version, requester};
      pilots.aircrafts(parameters, [](const auto&) {});
    }
  }
}
