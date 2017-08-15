#ifndef AIRMAP_REST_STATUS_H_
#define AIRMAP_REST_STATUS_H_

#include <airmap/status.h>

#include <airmap/rest/communicator.h>

namespace airmap {
namespace rest {

class Status : public airmap::Status {
 public:
  explicit Status(const std::string& host, Client::Version version, Communicator& communicator);

  void get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_polygon(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;

 private:
  std::string host_;
  Client::Version version_;
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_STATUS_H_
