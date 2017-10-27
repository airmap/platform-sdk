#ifndef AIRMAP_QT_AIRSPACES_H_
#define AIRMAP_QT_AIRSPACES_H_

#include <airmap/airspaces.h>
#include <airmap/client.h>
#include <airmap/qt/dispatcher.h>

#include <memory>

namespace airmap {
namespace qt {

class Airspaces : public airmap::Airspaces, public std::enable_shared_from_this<Airspaces> {
 public:
  explicit Airspaces(const std::shared_ptr<Dispatcher>& dispatcher, const std::shared_ptr<airmap::Client>& client);

  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) override;

 private:
  std::shared_ptr<Dispatcher> dispatcher_;
  std::shared_ptr<airmap::Client> client_;
};

}  // namespace qt
}  // namespace airmap

#endif  // AIRMAP_QT_AIRSPACES_H_
