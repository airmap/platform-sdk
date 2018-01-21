#ifndef AIRMAP_REST_TELEMETRY_H_
#define AIRMAP_REST_TELEMETRY_H_

#include <airmap/telemetry.h>

#include <airmap/net/udp/sender.h>

#include <memory>

namespace airmap {
namespace rest {
namespace detail {

class AES256Encryptor : public DoNotCopyOrMove {
 public:
  static const unsigned int block_size_in_bytes;
  static const unsigned int key_size_in_bytes;

  virtual std::string create_shared_secret() = 0;
  virtual std::string encrypt(const std::string& message, const std::string& key, const std::string& iv) = 0;

 protected:
  AES256Encryptor() = default;
};

class OpenSSLAES256Encryptor : public AES256Encryptor {
 public:
  static const bool is_openssl_initialized;

  OpenSSLAES256Encryptor();
  std::string create_shared_secret() override;
  std::string encrypt(const std::string& message, const std::string& key, const std::string& iv) override;
};

}  // namespace detail

class Telemetry : public airmap::Telemetry {
 public:
  explicit Telemetry(const std::shared_ptr<detail::AES256Encryptor>& encryptor,
                     const std::shared_ptr<net::udp::Sender>& sender);

  void submit_updates(const Flight& flight, const std::string& key,
                      const std::initializer_list<Update>& updates) override;

 private:
  std::shared_ptr<net::udp::Sender> sender_;
  std::shared_ptr<detail::AES256Encryptor> encryptor_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_TELEMETRY_H_
