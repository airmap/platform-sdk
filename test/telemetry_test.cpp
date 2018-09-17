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
#include <airmap/rest/telemetry.h>

#include <boost/beast/core/detail/base64.hpp>
#include <catch/catch.hpp>

namespace {
constexpr const char* key{"YWRkMTYzZGQ2YzE1ZDQ0M2IzYmM2MTQ2"};
constexpr const char* iv{"M2U4NTc1MTVjZjJmOGRjMzg0YTE5ZjNi"};
}  // namespace

TEST_CASE("telemetry") {
  SECTION("encryption") {
    SECTION("openssl encryptor encrypts correctly for key") {
      airmap::rest::detail::OpenSSLAES256Encryptor encryptor;
      REQUIRE(boost::beast::detail::base64_encode(encryptor.encrypt("AirMap is great", key, iv)) ==
              "72swQeeZxSJsQTJB3rRrjQ==");
    }
  }
}
