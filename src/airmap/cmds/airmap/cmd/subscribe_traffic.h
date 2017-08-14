#ifndef AIRMAP_CMDS_AIRMAP_CMD_SUBSCRIBE_TRAFFIC_H_
#define AIRMAP_CMDS_AIRMAP_CMD_SUBSCRIBE_TRAFFIC_H_

#include <airmap/traffic.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class SubscribeTraffic : public util::cli::CommandWithFlagsAndAction {
 public:
  SubscribeTraffic();

 private:
  using ApiKey        = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Authorization = util::TaggedString<util::tags::MustNotBeEmpty>;
  using FlightId      = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  struct {
    Optional<ApiKey> api_key;
    Optional<Authorization> authorization;
    Optional<FlightId> flight_id;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_SUBSCRIBE_TRAFFIC_H_
