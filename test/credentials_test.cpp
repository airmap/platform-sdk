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
#define BOOST_TEST_MODULE credentials

#include <airmap/credentials.h>

#include <boost/test/included/unit_test.hpp>

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
