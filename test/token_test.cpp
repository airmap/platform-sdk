#define BOOST_TEST_MODULE token

#include <airmap/token.h>

#include <boost/test/included/unit_test.hpp>

namespace airmap {

template <typename T>
std::ostream& operator<<(std::ostream& out, const T&) {
  return out;
}

}  // namespace airmap

BOOST_AUTO_TEST_CASE(token_default_ctor_yields_unknown_type) {
  airmap::Token token;
  BOOST_TEST(token.type() == airmap::Token::Type::unknown);
}

BOOST_AUTO_TEST_CASE(construction_for_anonymous_details_yields_anonymous_type) {
  airmap::Token token{airmap::Token::Anonymous{}};
  BOOST_TEST(token.type() == airmap::Token::Type::anonymous);
}

BOOST_AUTO_TEST_CASE(construction_for_oauth_details_yields_oauth_type) {
  airmap::Token token{airmap::Token::OAuth{}};
  BOOST_TEST(token.type() == airmap::Token::Type::oauth);
}

BOOST_AUTO_TEST_CASE(construction_for_refreshed_details_yields_refreshed_type) {
  airmap::Token token{airmap::Token::Refreshed{}};
  BOOST_TEST(token.type() == airmap::Token::Type::refreshed);
}

BOOST_AUTO_TEST_CASE(copy_construction_yields_correct_type) {
  airmap::Token original{airmap::Token::OAuth{}};
  airmap::Token copy{original};
  BOOST_TEST(copy.type() == original.type());
  BOOST_TEST(copy.oauth() == original.oauth());
}

BOOST_AUTO_TEST_CASE(assigning_tokens_alters_type_and_details) {
  airmap::Token::OAuth oauth{};
  airmap::Token::Refreshed refreshed{};
  airmap::Token t1{oauth};
  airmap::Token t2{refreshed};
  std::swap(t1, t2);
  BOOST_TEST(t1.type() == airmap::Token::Type::refreshed);
  BOOST_TEST(t1.refreshed() == refreshed);
  BOOST_TEST(t2.type() == airmap::Token::Type::oauth);
  BOOST_TEST(t2.oauth() == oauth);
}

BOOST_AUTO_TEST_CASE(type_is_correctly_inserted_into_output_stream) {
  {
    std::stringstream ss;
    ss << airmap::Token::Type::anonymous;
    BOOST_TEST(ss.str() == "anonymous");
  }
  {
    std::stringstream ss;
    ss << airmap::Token::Type::oauth;
    BOOST_TEST(ss.str() == "oauth");
  }
  {
    std::stringstream ss;
    ss << airmap::Token::Type::refreshed;
    BOOST_TEST(ss.str() == "refreshed");
  }
}
BOOST_AUTO_TEST_CASE(type_is_correctly_extracted_from_input_stream) {
  {
    std::stringstream ss{"anonymous"};
    airmap::Token::Type type{airmap::Token::Type::unknown};
    ss >> type;
    BOOST_TEST(type == airmap::Token::Type::anonymous);
  }
  {
    std::stringstream ss{"oauth"};
    airmap::Token::Type type{airmap::Token::Type::oauth};
    ss >> type;
    BOOST_TEST(type == airmap::Token::Type::oauth);
  }
  {
    std::stringstream ss{"refreshed"};
    airmap::Token::Type type{airmap::Token::Type::unknown};
    ss >> type;
    BOOST_TEST(type == airmap::Token::Type::refreshed);
  }
}

BOOST_AUTO_TEST_CASE(oauth_is_correctly_parsed_from_json) {
  auto json =
      R"_({"type": "oauth", "oauth": {"refresh_token": "refresh_token","id_token": "id_token","access_token": "access_token","token_type": "bearer"}})_";
  std::stringstream ss{json};
  auto token = airmap::Token::load_from_json(ss);
  BOOST_TEST(token.type() == airmap::Token::Type::oauth);
  BOOST_TEST(token.oauth().refresh == "refresh_token");
  BOOST_TEST(token.oauth().access == "access_token");
  BOOST_TEST(token.oauth().id == "id_token");
  BOOST_TEST(token.oauth().type == airmap::Token::OAuth::Type::bearer);
}

BOOST_AUTO_TEST_CASE(refreshed_is_correctly_parsed_from_json) {
  auto json =
      R"_({"type": "refreshed", "refreshed": {"id_token": "id_token","expires_in": 42,"token_type": "Bearer"}})_";
  std::stringstream ss{json};
  auto token = airmap::Token::load_from_json(ss);
  BOOST_TEST(token.type() == airmap::Token::Type::refreshed);
  BOOST_TEST(token.refreshed().expires_in.count() == 42);
  BOOST_TEST(token.refreshed().id == "id_token");
  BOOST_TEST(token.refreshed().type == airmap::Token::OAuth::Type::bearer);
}

BOOST_AUTO_TEST_CASE(anonymous_is_correctly_parsed_from_json) {
  auto json = R"_({"type": "anonymous", "anonymous": {"id_token": "id_token"}})_";
  std::stringstream ss{json};
  auto token = airmap::Token::load_from_json(ss);
  BOOST_TEST(token.type() == airmap::Token::Type::anonymous);
  BOOST_TEST(token.anonymous().id == "id_token");
}
