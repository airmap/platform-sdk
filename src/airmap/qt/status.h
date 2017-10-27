#ifndef AIRMAP_QT_STATUS_H_
#define AIRMAP_QT_STATUS_H_

#include <airmap/client.h>
#include <airmap/status.h>
#include <airmap/qt/dispatcher.h>

#include <memory>

namespace airmap {
namespace qt {

class Status : public airmap::Status {
 public:
  explicit Status(const std::shared_ptr<Dispatcher>& dispatcher, const std::shared_ptr<Client>& client);

  void get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_polygon(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;

 private:
  std::shared_ptr<Dispatcher> dispatcher_;
  std::shared_ptr<Client> client_;
};

}  // namespace qt
}  // namespace airmap

#endif  // AIRMAP_QT_STATUS_H_
