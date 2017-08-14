#ifndef AIRMAP_REST_TRAFFIC_H_
#define AIRMAP_REST_TRAFFIC_H_

#include <airmap/traffic.h>

#include <airmap/rest/communicator.h>

namespace airmap {
namespace rest {

class Traffic : public airmap::Traffic {
 public:
  explicit Traffic(Communicator& communicator);

  // TBD test only
  void subscribe(const Subscribe::Params& params, const Subscribe::Callback& cb) override;

 private:
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_TRAFFIC_H_
