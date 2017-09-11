#ifndef AIRMAP_PLATFORM_PATH_H_
#define AIRMAP_PLATFORM_PATH_H_

#include <boost/filesystem.hpp>

namespace airmap {
namespace platform {

using Path = ::boost::filesystem::path;
using ::boost::filesystem::current_path;
using ::boost::filesystem::initial_path;

}  // namespace platform
}  // namespace airmap

#endif  // AIRMAP_PLATFORM_PATH_H_
