#include <airmap/platform/linux/standard_paths.h>

#include <xdg/xdg.h>

airmap::Optional<airmap::platform::Path> airmap::platform::linux_::StandardPaths::path(Scope scope, Location location) {
  switch (location) {
    case Location::cache:
      switch (scope) {
        case Scope::system:
          return Optional<Path>{};
        case Scope::user:
          return xdg::cache().home();
      }
      break;
    case Location::config:
      switch (scope) {
        case Scope::system:
          return xdg::config().dirs().front();
        case Scope::user:
          return xdg::config().home();
      }
      break;
    case Location::data:
      switch (scope) {
        case Scope::system:
          return xdg::data().dirs().front();
        case Scope::user:
          return xdg::data().home();
      }
      break;
    case Location::runtime:
      switch (scope) {
        case Scope::system:
          return xdg::runtime().dir();
        case Scope::user:
          return Optional<Path>{};
      }
      break;
  }

  throw std::logic_error{"should not reach here"};
}
