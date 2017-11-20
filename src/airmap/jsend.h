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
      return Result{Error{"a failure occured, please see description for details"}.description(j.at(key::data).dump())};
    }

    if (s == status::error) {
      return Result{Error{j.at(key::message).get<std::string>()}.description(j.at(key::data).dump())};
    }
  } else {
    if (j.find(status::error) != j.end()) {
      return Result{Error{j.at(status::error).get<std::string>()}};
    }

    return Result{j.get<T>()};
  }

  return Result{Error{"not jsend formatted"}.value(Error::Value{"json"}, Error::Value{j.dump()})};
}

template <typename T>
inline Outcome<T, Error> parse_to_outcome(const std::string& json) {
  try {
    return to_outcome<T>(nlohmann::json::parse(json));
  } catch (const std::exception& e) {
    return Outcome<T, Error>{
        Error{"failed to parse JSON response"}.description(e.what()).value(Error::Value{"json"}, Error::Value{json})};
  }
}

}  // namespace jsend
}  // namespace airmap

#endif  // AIRMAP_JSEND_H_