#include <airmap/token.h>

#include <catch/catch.hpp>

TEST_CASE("airmap::Token") {
  SECTION("default c'tor yields unknown Token::Type") {
    airmap::Token token;
    REQUIRE(token.type() == airmap::Token::Type::unknown);
  }
  SECTION("construction for anonymous details yields anonymous Token::Type") {
    airmap::Token token{airmap::Token::Anonymous{}};
    REQUIRE(token.type() == airmap::Token::Type::anonymous);
  }
  SECTION("construction for oauth details yields oauth Token::Type") {
    airmap::Token token{airmap::Token::OAuth{}};
    REQUIRE(token.type() == airmap::Token::Type::oauth);
  }
  SECTION("construction for refreshed details yields refreshed Token::Type") {
    airmap::Token token{airmap::Token::Refreshed{}};
    REQUIRE(token.type() == airmap::Token::Type::refreshed);
  }
  SECTION("copy construction yields correct Token::Type") {
    airmap::Token original{airmap::Token::OAuth{}};
    airmap::Token copy{original};
    REQUIRE(copy.type() == original.type());
    REQUIRE(copy.oauth() == original.oauth());
  }
  SECTION("assigning tokens alters type and details") {
    airmap::Token::OAuth oauth{};
    airmap::Token::Refreshed refreshed{};
    airmap::Token t1{oauth};
    airmap::Token t2{refreshed};
    std::swap(t1, t2);
    REQUIRE(t1.type() == airmap::Token::Type::refreshed);
    REQUIRE(t1.refreshed() == refreshed);
    REQUIRE(t2.type() == airmap::Token::Type::oauth);
    REQUIRE(t2.oauth() == oauth);
  }
  SECTION("Token::Type is correctly inserted into output stream") {
    {
      std::stringstream ss;
      ss << airmap::Token::Type::anonymous;
      REQUIRE(ss.str() == "anonymous");
    }
    {
      std::stringstream ss;
      ss << airmap::Token::Type::oauth;
      REQUIRE(ss.str() == "oauth");
    }
    {
      std::stringstream ss;
      ss << airmap::Token::Type::refreshed;
      REQUIRE(ss.str() == "refreshed");
    }
  }
  SECTION("Token::Type is correctly extracted from input stream") {
    {
      std::stringstream ss{"anonymous"};
      airmap::Token::Type type{airmap::Token::Type::unknown};
      ss >> type;
      REQUIRE(type == airmap::Token::Type::anonymous);
    }
    {
      std::stringstream ss{"oauth"};
      airmap::Token::Type type{airmap::Token::Type::oauth};
      ss >> type;
      REQUIRE(type == airmap::Token::Type::oauth);
    }
    {
      std::stringstream ss{"refreshed"};
      airmap::Token::Type type{airmap::Token::Type::unknown};
      ss >> type;
      REQUIRE(type == airmap::Token::Type::refreshed);
    }
  }
  SECTION("Token::Type::oauth is correctly parsed from JSON") {
    auto json =
        R"_({"type": "oauth", "oauth": {"refresh_token": "refresh_token","id_token": "id_token","access_token": "access_token","token_type": "bearer"}})_";
    std::stringstream ss{json};
    auto token = airmap::Token::load_from_json(ss);
    REQUIRE(token.type() == airmap::Token::Type::oauth);
    REQUIRE(token.oauth().refresh == "refresh_token");
    REQUIRE(token.oauth().access == "access_token");
    REQUIRE(token.oauth().id == "id_token");
    REQUIRE(token.oauth().type == airmap::Token::OAuth::Type::bearer);
  }
  SECTION("Token::Type::refreshed is correctly parsed from JSON") {
    auto json =
        R"_({"type": "refreshed", "refreshed": {"id_token": "id_token","expires_in": 42,"token_type": "bearer"}})_";
    std::stringstream ss{json};
    auto token = airmap::Token::load_from_json(ss);
    REQUIRE(token.type() == airmap::Token::Type::refreshed);
    REQUIRE(token.refreshed().expires_in.count() == 42);
    REQUIRE(token.refreshed().id == "id_token");
    REQUIRE(token.refreshed().type == airmap::Token::Refreshed::Type::bearer);
  }
  SECTION("Token::Type::anonymous is correctly parsed from JSON") {
    auto json = R"_({"type": "anonymous", "anonymous": {"id_token": "id_token"}})_";
    std::stringstream ss{json};
    auto token = airmap::Token::load_from_json(ss);
    REQUIRE(token.type() == airmap::Token::Type::anonymous);
    REQUIRE(token.anonymous().id == "id_token");
  }
}
