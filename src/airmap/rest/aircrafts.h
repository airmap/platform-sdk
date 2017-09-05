#ifndef AIRMAP_REST_AIRCRAFTS_H_
#define AIRMAP_REST_AIRCRAFTS_H_

#include <airmap/aircrafts.h>
#include <airmap/client.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

class Aircrafts : public airmap::Aircrafts {
 public:
  explicit Aircrafts(Client::Version version, const std::shared_ptr<net::http::Requester>& requester);

  void manufacturers(const Manufacturers::Parameters& parameters, const Manufacturers::Callback& cb) override;
  void models(const Models::Parameters& parameters, const Models::Callback& cb) override;
  void model_for_id(const ModelForId::Parameters& parameters, const ModelForId::Callback& cb) override;

 private:
  Client::Version version_;
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AIRCRAFTS_H_
