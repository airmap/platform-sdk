#ifndef AIRMAP_CMDS_AIRMAP_CMD_GET_STATUS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_GET_STATUS_H_

#include <airmap/client.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/status.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class GetStatus : public util::cli::CommandWithFlagsAndAction {
 public:
  GetStatus();

 private:
  using ApiKey = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Optional<ApiKey> api_key_;
  Status::GetStatus::Parameters params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_GET_STATUS_H_
