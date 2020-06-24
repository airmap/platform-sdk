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

#include <airmap/flight.h>
#include <airmap/util/fmt.h>

#include <airmap/pregenerated/telemetry.pb.h>

#include <boost/beast/core/detail/base64.hpp>

#include <openssl/conf.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <arpa/inet.h>

#include <cstdint>
#include <cstdlib>
#include <sstream>

namespace base64 = boost::beast::detail::base64;
namespace fmt    = airmap::util::fmt;

namespace {

namespace openssl {

bool init_once() {
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);
  CRYPTO_malloc_init();
  // A word on seeding the PRNG used by SSL. On all the platforms we
  // care about, the PRNG is transparently seeded by using underlying
  // platform facilities, e.g., /dev/urandom on Posix-like platforms.
  // For that, we do not explicitly seed here and instead let OpenSSL
  // tell us loudly if not enough entropy is available.
  return true;
}

}  // namespace openssl

class Buffer {
 public:
  template <typename T>
  Buffer& add(const T& value) {
    return add(reinterpret_cast<const char*>(&value), sizeof(value));
  }

  Buffer& add(const std::string& s) {
    return add(s.c_str(), s.size());
  }

  Buffer& add(const std::vector<std::uint8_t>& v) {
    return add(reinterpret_cast<const char*>(v.data()), v.size());
  }

  Buffer& add(const char* value, std::size_t size) {
    buffer_.insert(buffer_.end(), value, value + size);
    return *this;
  }

  const std::string& get() const {
    return buffer_;
  }

 private:
  std::string buffer_;
};

namespace telemetry {

constexpr std::uint8_t encryption_type{1};

}  // namespace telemetry
}  // namespace

const unsigned int airmap::rest::detail::AES256Encryptor::block_size_in_bytes   = 16;
const unsigned int airmap::rest::detail::AES256Encryptor::key_size_in_bytes     = 32;
const bool airmap::rest::detail::OpenSSLAES256Encryptor::is_openssl_initialized = openssl::init_once();

airmap::rest::detail::OpenSSLAES256Encryptor::OpenSSLAES256Encryptor() {
}

std::string airmap::rest::detail::OpenSSLAES256Encryptor::create_shared_secret() {
  std::string iv(block_size_in_bytes, 0);
  if (RAND_bytes(reinterpret_cast<unsigned char*>(&iv[0]), iv.size()) != 1) {
    // We are very vocal about an error here. RAND_bytes reproting an
    // error indicates insufficient entropy to create a cryptographically
    // strong blob of bytes. Better safe than sorry and bail out.
    throw std::runtime_error{ERR_error_string(ERR_get_error(), nullptr)};
  }
  return iv;
}

std::string airmap::rest::detail::OpenSSLAES256Encryptor::encrypt(const std::string& message, const std::string& key,
                                                                  const std::string& iv) {
  std::string dest;
  dest.resize(base64::decoded_size(key.size()));
  auto const result = base64::decode(&dest[0], key.data(), key.size());
  dest.resize(result.first);

  auto decoded_key = dest;

  std::shared_ptr<EVP_CIPHER_CTX> ctx{EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free};
  if (not ctx) {
    throw std::runtime_error{"failed to create encryption context"};
  }

  if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr,
                         reinterpret_cast<const unsigned char*>(decoded_key.data()),
                         reinterpret_cast<const unsigned char*>(iv.data())) != 1) {
    throw std::runtime_error{"failed to initialize encryption context"};
  }

  const auto payload_size = static_cast<int>(message.size());
  auto payload_data       = reinterpret_cast<const unsigned char*>(message.data());

  std::string cipher(payload_size + block_size_in_bytes, '0');

  const auto cipher_size = static_cast<int>(cipher.size());
  auto cipher_data_begin = reinterpret_cast<unsigned char*>(&cipher[0]);
  auto cipher_data       = cipher_data_begin;
  auto available         = cipher_size;

  if (EVP_EncryptUpdate(ctx.get(), cipher_data, &available, payload_data, payload_size) != 1) {
    throw std::runtime_error{"failed to update encryption context for data"};
  }

  cipher_data = cipher_data + available;
  available   = cipher_size - available;

  if (EVP_EncryptFinal_ex(ctx.get(), cipher_data, &available) != 1) {
    throw std::runtime_error{"failed to finalize encryption"};
  }

  cipher_data = cipher_data + available;

  return std::string{cipher.begin(), cipher.begin() + std::distance(cipher_data_begin, cipher_data)};
}

airmap::rest::Telemetry::Telemetry(const std::shared_ptr<detail::AES256Encryptor>& encryptor,
                                   const std::shared_ptr<net::udp::Sender>& sender)
    : sender_{sender}, encryptor_{encryptor} {
}

void airmap::rest::Telemetry::submit_updates(const Flight& flight, const std::string& key,
                                             const std::initializer_list<Update>& updates) {
  static std::uint32_t counter{1};

  Buffer payload;

  for (const auto& update : updates) {
    switch (update.type()) {
      case Update::Type::position: {
        airmap::telemetry::Position position;
        position.set_timestamp(update.position().timestamp);
        position.set_latitude(update.position().latitude);
        position.set_longitude(update.position().longitude);
        position.set_altitude_agl(update.position().altitude_gl);
        position.set_altitude_msl(update.position().altitude_msl);
        position.set_horizontal_accuracy(update.position().horizontal_accuracy);
        auto message = position.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::position)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
      case Update::Type::speed: {
        airmap::telemetry::Speed speed;
        speed.set_timestamp(update.speed().timestamp);
        speed.set_velocity_x(update.speed().velocity_x);
        speed.set_velocity_y(update.speed().velocity_y);
        speed.set_velocity_z(update.speed().velocity_z);
        auto message = speed.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::speed)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
      case Update::Type::attitude: {
        airmap::telemetry::Attitude attitude;
        attitude.set_timestamp(update.attitude().timestamp);
        attitude.set_yaw(update.attitude().yaw);
        attitude.set_pitch(update.attitude().pitch);
        attitude.set_roll(update.attitude().roll);
        auto message = attitude.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::attitude)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
      case Update::Type::barometer: {
        airmap::telemetry::Barometer barometer;
        barometer.set_timestamp(update.barometer().timestamp);
        barometer.set_pressure(update.barometer().pressure);
        auto message = barometer.SerializeAsString();
        payload.add<std::uint16_t>(htons(static_cast<std::uint16_t>(Telemetry::Update::Type::barometer)))
            .add<std::uint16_t>(htons(message.size()))
            .add(message);
        break;
      }
    }
  }

  auto iv  = encryptor_->create_shared_secret();
  auto msg = encryptor_->encrypt(payload.get(), key, iv);

  Buffer packet;
  sender_->send(packet.add<std::uint32_t>(htonl(counter++))
                    .add<std::uint8_t>(flight.id.size())
                    .add(flight.id)
                    .add<std::uint8_t>(::telemetry::encryption_type)
                    .add(iv)
                    .add(msg)
                    .get(),
                [](const auto&) {});
}
