#define CATCH_CONFIG_MAIN

#include <airmap/rest/aircrafts.h>
#include <airmap/rest/airspaces.h>
#include <airmap/rest/communicator.h>
#include <airmap/rest/flights.h>

#include <airmap/codec.h>

#include <catch/catch.hpp>
#include <trompeloeil/trompeloeil.hpp>

namespace mock = trompeloeil;

namespace {

using StringMap = std::unordered_map<std::string, std::string>;

struct MockCommunicator : public airmap::rest::Communicator {
  MAKE_MOCK5(get, void(const std::string&, const std::string&, StringMap&&, StringMap&&, DoCallback), override);
  MAKE_MOCK5(post, void(const std::string&, const std::string&, StringMap&&, const std::string&, DoCallback), override);
  MAKE_MOCK3(send_udp, void(const std::string&, std::uint16_t, const std::string&), override);
  MAKE_MOCK1(dispatch, void(const std::function<void()>&), override);
};

}  // namespace

TEST_CASE("rest") {
  using mock::_;

  SECTION("aircrafts") {
    SECTION("search for manufactureres issues get request with correct parameters") {
      airmap::Aircrafts::Manufacturers::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      MockCommunicator communicator;
      REQUIRE_CALL(communicator,
                   get(mock::eq<std::string>("api.airmap.com"), mock::eq<std::string>("/aircraft/v2/manufacturer"),
                       mock::eq<StringMap>(query), ANY(StringMap), ANY(airmap::rest::Communicator::DoCallback)));

      airmap::rest::Aircrafts aircrafts{communicator};
      aircrafts.manufacturers(parameters, [](const airmap::Aircrafts::Manufacturers::Result&) {});
    }

    SECTION("search for models issues get request with correct parameters") {
      airmap::Aircrafts::Models::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      MockCommunicator communicator;
      REQUIRE_CALL(communicator,
                   get(mock::eq<std::string>("api.airmap.com"), mock::eq<std::string>("/aircraft/v2/model"),
                       mock::eq<StringMap>(query), ANY(StringMap), ANY(airmap::rest::Communicator::DoCallback)));

      airmap::rest::Aircrafts aircrafts{communicator};
      aircrafts.models(parameters, [](const airmap::Aircrafts::Models::Result&) {});
    }

    SECTION("search for model by id issues get request with correct parameters") {
      airmap::Aircrafts::ModelForId::Parameters parameters;
      parameters.id = "42";

      MockCommunicator communicator;
      REQUIRE_CALL(communicator, get(mock::eq<std::string>("api.airmap.com"),
                                     mock::eq<std::string>("/aircraft/v2/model/" + parameters.id), ANY(StringMap),
                                     ANY(StringMap), ANY(airmap::rest::Communicator::DoCallback)));

      airmap::rest::Aircrafts aircrafts{communicator};
      aircrafts.model_for_id(parameters, [](const airmap::Aircrafts::ModelForId::Result&) {});
    }
  }

  SECTION("airspaces") {
    SECTION("search issues get request with correct parameters") {
      airmap::Airspaces::Search::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      MockCommunicator communicator;
      REQUIRE_CALL(communicator,
                   get(mock::eq<std::string>("api.airmap.com"), mock::eq<std::string>("/airspace/v2/search"),
                       mock::eq<StringMap>(query), ANY(StringMap), ANY(airmap::rest::Communicator::DoCallback)));

      airmap::rest::Airspaces airspaces{communicator};
      airspaces.search(parameters, [](const airmap::Airspaces::Search::Result&) {});
    }
    SECTION("for id issues get request with correct parameters") {
      airmap::Airspaces::ForIds::Parameters parameters;
      parameters.id = "42";

      MockCommunicator communicator;
      REQUIRE_CALL(communicator,
                   get(mock::eq<std::string>("api.airmap.com"), mock::eq<std::string>("/airspace/v2/" + parameters.id),
                       ANY(StringMap), ANY(StringMap), ANY(airmap::rest::Communicator::DoCallback)));

      airmap::rest::Airspaces airspaces{communicator};
      airspaces.for_ids(parameters, [](const airmap::Airspaces::ForIds::Result&) {});
    }
  }

  SECTION("flights") {
    SECTION("search issues get request with correct parameters") {
      airmap::Flights::Search::Parameters parameters;
      StringMap query;
      airmap::codec::http::query::encode(query, parameters);

      MockCommunicator communicator;
      REQUIRE_CALL(communicator,
                   get(mock::eq<std::string>("api.airmap.com"), mock::eq<std::string>("/flight/v2"),
                       mock::eq<StringMap>(query), ANY(StringMap), ANY(airmap::rest::Communicator::DoCallback)));

      airmap::rest::Flights flights{communicator};
      flights.search(parameters, [](const airmap::Flights::Search::Result&) {});
    }
    SECTION("for id issues get request with correct parameters") {
      airmap::Flights::ForId::Parameters parameters;
      parameters.id = "flight|abc";

      MockCommunicator communicator;
      REQUIRE_CALL(communicator,
                   get(mock::eq<std::string>("api.airmap.com"), mock::eq<std::string>("/flight/v2/" + parameters.id),
                       ANY(StringMap), ANY(StringMap), ANY(airmap::rest::Communicator::DoCallback)));

      airmap::rest::Flights flights{communicator};
      flights.for_id(parameters, [](const airmap::Flights::ForId::Result&) {});
    }
  }
}
