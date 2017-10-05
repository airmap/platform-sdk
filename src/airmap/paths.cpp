#include <airmap/paths.h>

#include <airmap/platform/interface.h>
#include <airmap/platform/standard_paths.h>

airmap::platform::Path airmap::paths::config_dir(Client::Version version) {
  auto p = airmap::platform::create_interface();
  switch (version) {
    case Client::Version::production:
      return p->standard_paths()
                 .path(platform::StandardPaths::Scope::user, platform::StandardPaths::Location::config)
                 .get() /
             "airmap" / "production";
    case Client::Version::staging:
      return p->standard_paths()
                 .path(platform::StandardPaths::Scope::user, platform::StandardPaths::Location::config)
                 .get() /
             "airmap" / "staging";
  }

  throw std::logic_error{"should not reach here"};
}

airmap::platform::Path airmap::paths::token_dir(Client::Version version) {
  return config_dir(version) / "token";
}

airmap::platform::Path airmap::paths::config_file(Client::Version version) {
  return config_dir(version) / "config.json";
}

airmap::platform::Path airmap::paths::anonymous_token_file(Client::Version version) {
  return token_dir(version) / "anonymous.json";
}

airmap::platform::Path airmap::paths::oauth_token_file(Client::Version version) {
  return token_dir(version) / "oauth.json";
}

airmap::platform::Path airmap::paths::refreshed_token_file(Client::Version version) {
  return token_dir(version) / "refreshed.json";
}
