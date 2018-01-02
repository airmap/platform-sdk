#include <airmap/net/http/user_agent.h>

#include <airmap/util/fmt.h>
#include <airmap/version.h>

std::string airmap::net::http::user_agent() {
  return util::fmt::sprintf("AirMap/%d.%d.%d", Version::current().major, Version::current().minor,
                            Version::current().patch);
}
