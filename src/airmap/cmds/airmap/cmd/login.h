/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  login.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_LOGIN_H_
#define AIRMAP_CMDS_AIRMAP_CMD_LOGIN_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/credentials.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/status.h>
#include <airmap/token.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Login : public util::cli::CommandWithFlagsAndAction {
 public:
  Login();

 private:
  using ConstContextRef = std::reference_wrapper<const util::cli::Command::Context>;

  void renew_authentication(const Credentials& credentials, const Token& token, ConstContextRef context);
  void request_authentication(const Credentials& credentials, ConstContextRef context);
  void handle_result_for_authentication_with_password(const Authenticator::AuthenticateWithPassword::Result& result,
                                                      ConstContextRef context);
  void handle_result_for_anonymous_authentication(const Authenticator::AuthenticateAnonymously::Result& result,
                                                  ConstContextRef context);
  void handle_result_for_renewed_authentication(const Authenticator::RenewAuthentication::Result& result,
                                                const Token& previous_token, ConstContextRef context);

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Optional<ConfigFile> config_file_;
  Optional<TokenFile> token_file_;
  bool renew_{false};
  std::shared_ptr<::airmap::Context> context_;
  std::shared_ptr<Client> client_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_LOGIN_H_
