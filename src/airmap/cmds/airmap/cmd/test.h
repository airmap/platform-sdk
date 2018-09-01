//
//  test.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_TEST_H_
#define AIRMAP_CMDS_AIRMAP_CMD_TEST_H_

#include <airmap/client.h>
#include <airmap/cmds/airmap/cmd/flags.h>
#include <airmap/context.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/token.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

#include <functional>
#include <memory>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Test : public util::cli::CommandWithFlagsAndAction {
 public:
  class Suite : DoNotCopyOrMove {
   public:
    using Factory = std::function<std::shared_ptr<Suite>()>;

    virtual void run(const std::shared_ptr<Logger>& logger, const std::shared_ptr<::airmap::Client>& client,
                     const std::shared_ptr<::airmap::Context>& context, const ::airmap::Token& token) = 0;

   protected:
    Suite() = default;
  };

  Test();

 private:
  using TestSuite = util::TaggedString<util::tags::MustNotBeEmpty>;

  std::map<std::string, Suite::Factory> test_suite_registry_;
  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Optional<ConfigFile> config_file_;
  Optional<TokenFile> token_file_;
  Optional<TestSuite> test_suite_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_TEST_H_
