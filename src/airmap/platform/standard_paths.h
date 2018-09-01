//
//  standard_paths.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_PLATFORM_STANDARD_PATHS_H_
#define AIRMAP_PLATFORM_STANDARD_PATHS_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>
#include <airmap/platform/path.h>

#include <iosfwd>

namespace airmap {
namespace platform {

class StandardPaths : DoNotCopyOrMove {
 public:
  enum class Scope { system, user };

  enum class Location { cache, config, data, runtime };

  virtual Optional<Path> path(Scope scope, Location location) = 0;

 protected:
  StandardPaths() = default;
};

std::ostream& operator<<(std::ostream&, StandardPaths::Scope);
std::ostream& operator<<(std::ostream&, StandardPaths::Location);

}  // namespace platform
}  // namespace airmap

#endif  // AIRMAP_PLATFORM_STANDARD_PATHS_H_
