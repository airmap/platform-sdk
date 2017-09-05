#ifndef AIRMAP_REST_STATUS_H_
#define AIRMAP_REST_STATUS_H_

#include <airmap/client.h>
#include <airmap/net/http/requester.h>
#include <airmap/status.h>

namespace airmap {
namespace rest {

class Status : public airmap::Status {
 public:
  explicit Status(Client::Version version, const std::shared_ptr<net::http::Requester>& requester);

  void get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_polygon(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;

 private:
  Client::Version version_;
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_STATUS_H_
