#ifndef AIRMAP_CMDS_AIRMAP_CMD_CREATE_FLIGHT_H_
#define AIRMAP_CMDS_AIRMAP_CMD_CREATE_FLIGHT_H_

#include <airmap/flights.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class CreateFlight : public util::cli::CommandWithFlagsAndAction {
 public:
  CreateFlight();

 private:
  std::string api_key_;
  Optional<std::string> geometry_file_;
  Flights::CreateFlight::Parameters params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_CREATE_FLIGHT_H_
