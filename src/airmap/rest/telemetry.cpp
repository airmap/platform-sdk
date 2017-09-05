#include <airmap/rest/telemetry.h>

#include <airmap/flight.h>

#include "telemetry.pb.h"

#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/ccm.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

#include <arpa/inet.h>

#include <stdlib.h>

#include <cstdint>
#include <sstream>

namespace {

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

airmap::rest::Telemetry::Telemetry(const std::shared_ptr<net::udp::Sender>& sender) : sender_{sender} {
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

  std::string cipher;

  using byte = unsigned char;

  std::vector<std::uint8_t> iv(16, 0);
  rng_.GenerateBlock(iv.data(), iv.size());

  std::string decoded_key;
  CryptoPP::StringSource decoder(key, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decoded_key)));

  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
  enc.SetKeyWithIV(reinterpret_cast<const byte*>(decoded_key.c_str()), decoded_key.size(), iv.data());

  CryptoPP::StringSource s(payload.get(), true,
                           new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::StringSink(cipher)));

  Buffer packet;
  sender_->send(packet.add<std::uint32_t>(htonl(counter++))
                    .add<std::uint8_t>(flight.id.size())
                    .add(flight.id)
                    .add<std::uint8_t>(::telemetry::encryption_type)
                    .add(iv)
                    .add(cipher)
                    .get(),
                [](const auto&) {});
}
