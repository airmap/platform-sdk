#ifndef AIRMAP_REST_TELEMETRY_H_
#define AIRMAP_REST_TELEMETRY_H_

#include <airmap/telemetry.h>

#include <airmap/net/udp/sender.h>

#include <cryptopp/osrng.h>

#include <memory>

namespace airmap {
namespace rest {

class Telemetry : public airmap::Telemetry {
 public:
  explicit Telemetry(const std::shared_ptr<net::udp::Sender>& sender);

  void submit_updates(const Flight& flight, const std::string& key,
                      const std::initializer_list<Update>& updates) override;

 private:
  std::shared_ptr<net::udp::Sender> sender_;
  CryptoPP::AutoSeededRandomPool rng_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_TELEMETRY_H_
