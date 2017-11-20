#ifndef AIRMAP_JSEND_H_
#define AIRMAP_JSEND_H_

#include <airmap/error.h>
#include <airmap/outcome.h>

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace jsend {
namespace status {

static constexpr const char* success{"success"};
static constexpr const char* error{"error"};
static constexpr const char* failure{"fail"};

}  // namespace status

namespace key {

static constexpr const char* status{"status"};
static constexpr const char* data{"data"};
static constexpr const char* message{"message"};

}  // namespace key

template <typename T>
inline Outcome<T, Error> to_outcome(const nlohmann::json& j) {
  using Result = Outcome<T, Error>;

  if (j.find(key::status) != j.end()) {
    auto s = j[key::status];

    if (s == status::success) {
      return Result{j[key::data].get<T>()};
    }

    if (s == status::failure) {
      Error err;
      err.description = j.at(key::data).dump();

      return Result{err};
    }

    if (s == status::error) {
      Error err;
      err.message     = j.at(key::message).get<std::string>();
      err.description = j.at(key::data).dump();

      return Result{err};
    }
  } else {
    if (j.find(status::error) != j.end()) {
      Error err;
      err.message = j.at(status::error).get<std::string>();
    }

    return Result{j.get<T>()};
  }

  Error err;
  err.message        = "not JSend formatted";
  err.values["json"] = Error::Value{j.dump()};

  return Result{err};
}

}  // namespace jsend
}  // namespace airmap

#endif  // AIRMAP_JSEND_H_