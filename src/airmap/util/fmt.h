#ifndef AIRMAP_UTIL_FMT_H_
#define AIRMAP_UTIL_FMT_H_

#include <boost/format.hpp>

#include <string>

namespace airmap {
namespace util {
namespace fmt {

template <typename Head>
std::string sprintf(boost::format& fmt, Head&& head) {
  return (fmt % head).str();
}

template <typename Head, typename... Tail>
std::string sprintf(boost::format& fmt, Head&& head, Tail&&... tail) {
  return sprintf(fmt % head, std::forward<Tail>(tail)...);
}

template <typename... Args>
inline std::string sprintf(const char* format, Args&&... args) {
  boost::format fmt{format};
  return sprintf(fmt, std::forward<Args>(args)...);
}

inline std::string sprintf(const char* format) {
  return format;
}

}  // namespace fmt
}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_FMT_H_