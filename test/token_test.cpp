// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
  BOOST_CHECK(token.type() == airmap::Token::Type::unknown);
}

BOOST_AUTO_TEST_CASE(construction_for_anonymous_details_yields_anonymous_type) {
  airmap::Token token{airmap::Token::Anonymous{}};
  BOOST_CHECK(token.type() == airmap::Token::Type::anonymous);
}

BOOST_AUTO_TEST_CASE(construction_for_oauth_details_yields_oauth_type) {
  airmap::Token token{airmap::Token::OAuth{}};
  BOOST_CHECK(token.type() == airmap::Token::Type::oauth);
}

BOOST_AUTO_TEST_CASE(construction_for_refreshed_details_yields_refreshed_type) {
  airmap::Token token{airmap::Token::Refreshed{}};
  BOOST_CHECK(token.type() == airmap::Token::Type::refreshed);
}

BOOST_AUTO_TEST_CASE(copy_construction_yields_correct_type) {
  airmap::Token original{airmap::Token::OAuth{}};
  airmap::Token copy{original};
  BOOST_CHECK(copy.type() == original.type());
  BOOST_CHECK(copy.oauth() == original.oauth());
}

BOOST_AUTO_TEST_CASE(assigning_tokens_alters_type_and_details) {
  airmap::Token::OAuth oauth{};
  airmap::Token::Refreshed refreshed{};
  airmap::Token t1{oauth};
  airmap::Token t2{refreshed};
  std::swap(t1, t2);
  BOOST_CHECK(t1.type() == airmap::Token::Type::refreshed);
  BOOST_CHECK(t1.refreshed() == refreshed);
  BOOST_CHECK(t2.type() == airmap::Token::Type::oauth);
  BOOST_CHECK(t2.oauth() == oauth);
}

BOOST_AUTO_TEST_CASE(type_is_correctly_inserted_into_output_stream) {
  {
    std::stringstream ss;
    ss << airmap::Token::Type::anonymous;
    BOOST_CHECK(ss.str() == "anonymous");
  }
  {
    std::stringstream ss;
    ss << airmap::Token::Type::oauth;
    BOOST_CHECK(ss.str() == "oauth");
  }
  {
    std::stringstream ss;
    ss << airmap::Token::Type::refreshed;
    BOOST_CHECK(ss.str() == "refreshed");
  }
}
BOOST_AUTO_TEST_CASE(type_is_correctly_extracted_from_input_stream) {
  {
    std::stringstream ss{"anonymous"};
    airmap::Token::Type type{airmap::Token::Type::unknown};
    ss >> type;
    BOOST_CHECK(type == airmap::Token::Type::anonymous);
  }
  {
    std::stringstream ss{"oauth"};
    airmap::Token::Type type{airmap::Token::Type::oauth};
    ss >> type;
    BOOST_CHECK(type == airmap::Token::Type::oauth);
  }
  {
    std::stringstream ss{"refreshed"};
    airmap::Token::Type type{airmap::Token::Type::unknown};
    ss >> type;
    BOOST_CHECK(type == airmap::Token::Type::refreshed);
  }
}

BOOST_AUTO_TEST_CASE(oauth_is_correctly_parsed_from_json) {
  auto json =
      R"_({"type": "oauth", "oauth": {"refresh_token": "refresh_token","id_token": "id_token","access_token": "access_token","token_type": "bearer"}})_";
  std::stringstream ss{json};
  auto token = airmap::Token::load_from_json(ss);
  BOOST_CHECK(token.type() == airmap::Token::Type::oauth);
  BOOST_CHECK(token.oauth().refresh == "refresh_token");
  BOOST_CHECK(token.oauth().access == "access_token");
  BOOST_CHECK(token.oauth().id == "id_token");
  BOOST_CHECK(token.oauth().type == airmap::Token::OAuth::Type::bearer);
}

BOOST_AUTO_TEST_CASE(refreshed_is_correctly_parsed_from_json) {
  auto json =
      R"_({"type": "refreshed", "refreshed": {"id_token": "id_token","expires_in": 42,"token_type": "Bearer"}})_";
  std::stringstream ss{json};
  auto token = airmap::Token::load_from_json(ss);
  BOOST_CHECK(token.type() == airmap::Token::Type::refreshed);
  BOOST_CHECK(token.refreshed().expires_in.count() == 42);
  BOOST_CHECK(token.refreshed().id == "id_token");
  BOOST_CHECK(token.refreshed().type == airmap::Token::OAuth::Type::bearer);
}

BOOST_AUTO_TEST_CASE(anonymous_is_correctly_parsed_from_json) {
  auto json = R"_({"type": "anonymous", "anonymous": {"id_token": "id_token"}})_";
  std::stringstream ss{json};
  auto token = airmap::Token::load_from_json(ss);
  BOOST_CHECK(token.type() == airmap::Token::Type::anonymous);
  BOOST_CHECK(token.anonymous().id == "id_token");
}
