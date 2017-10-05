#ifndef AIRMAP_PATHS_H_
#define AIRMAP_PATHS_H_

#include <airmap/client.h>
#include <airmap/platform/path.h>

namespace airmap {
namespace paths {

platform::Path config_dir(Client::Version version);
platform::Path token_dir(Client::Version version);
platform::Path config_file(Client::Version version);
platform::Path anonymous_token_file(Client::Version version);
platform::Path oauth_token_file(Client::Version version);
platform::Path refreshed_token_file(Client::Version version);

}  // namespace paths
}  // namespace airmap

#endif  // AIRMAP_PATHS_H_
