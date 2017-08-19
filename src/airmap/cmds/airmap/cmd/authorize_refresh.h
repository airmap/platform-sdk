#ifndef AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_REFRESH_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_REFRESH_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class AuthorizeRefresh : public util::cli::CommandWithFlagsAndAction {
 public:
  AuthorizeRefresh();

 private:
  using RefreshToken = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  struct {
    Client::Version version{Client::Version::production};
    Logger::Severity log_level{Logger::Severity::info};
    Optional<ApiKey> api_key;
    Optional<ClientId> client_id;
    Optional<RefreshToken> refresh_token;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_REFRESH_H_
