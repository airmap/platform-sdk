#define BOOST_TEST_MODULE credentials

#include <airmap/credentials.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_CASE(credentials_type_is_correctly_inserted_into_output_stream) {
  {
    std::stringstream ss;
    ss << airmap::Credentials::Type::anonymous;
    BOOST_CHECK(ss.str() == "anonymous");
  }
  {
    std::stringstream ss;
    ss << airmap::Credentials::Type::oauth;
    BOOST_CHECK(ss.str() == "oauth");
  }
}

BOOST_AUTO_TEST_CASE(credentials_type_is_correctly_extracted_from_input_stream) {
  {
    airmap::Credentials::Type type{airmap::Credentials::Type::oauth};
    std::stringstream ss{"anonymous"};
    ss >> type;
    BOOST_CHECK(type == airmap::Credentials::Type::anonymous);
  }
  {
    airmap::Credentials::Type type{airmap::Credentials::Type::anonymous};
    std::stringstream ss{"oauth"};
    ss >> type;
    BOOST_CHECK(type == airmap::Credentials::Type::oauth);
  }
}
