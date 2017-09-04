#define CATCH_CONFIG_MAIN

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
