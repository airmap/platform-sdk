#ifndef AIRMAP_REST_AIRSPACES_H_
#define AIRMAP_REST_AIRSPACES_H_

#include <airmap/airspaces.h>
#include <airmap/client.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

class Airspaces : public airmap::Airspaces {
 public:
  explicit Airspaces(Client::Version version, const std::shared_ptr<net::http::Requester>& requester);

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) override;

 private:
  Client::Version version_;
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AIRSPACES_H_
