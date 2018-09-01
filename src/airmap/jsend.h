//
//  jsend.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

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
static constexpr const char* msg{"msg"};
static constexpr const char* message{"message"};

}  // namespace key

// to_outcome analyzes a JSON document, trying to use it according
// to the JSEND spec. However, if and when the APIs in question do not conform,
// we put heuristics to handle that case here. At least, we will gather all the black
// magic in one place for future analysis.
template <typename T>
inline Outcome<T, Error> to_outcome(const nlohmann::json& j) {
  using Result = Outcome<T, Error>;

  if (j.find(key::status) != j.end()) {
    auto s = j[key::status];

    if (s == status::success) {
      return Result{j[key::data].get<T>()};
    } else if (s == status::failure) {
      if (j.count(key::message) > 0) {
        // This shouldn't happen. Failure responses are _not_ required
        // to carry a message. We have seen it in the wild, though.
        return Result{Error{j.at(key::message).get<std::string>()}};
      } else if (j.at(key::data).count(key::message) > 0) {
        // This is quite common in the wild, wrapping up the message in a 'data' object.
        return Result{Error{j.at(key::data).at(key::message).get<std::string>()}.description(j.at(key::data).dump())};
      }
      // Handling the general case gracefully, just dumping the JSON and indicating the issue.
      auto e = Error{"a failure occured, please see description for details"};

      if (j.count(key::data) > 0) {
        e = e.description(j.at(key::data).dump());
      }

      return Result{e};
    } else if (s == status::error) {
      if (j.count(key::message) > 0) {
        // This is the path according to the JSEND spec.
        auto e = Error{j.at(key::message).get<std::string>()};

        if (j.count(key::data) > 0) {
          e = e.description(j.at(key::data).dump());
        }

        return Result{e};
      }
      // Handling the general case gracefully again.
      auto e = Error{"an error occured, please see description for details"};

      if (j.count(key::data) > 0) {
        e = e.description(j.at(key::data).dump());
      }

      return Result{e};
    }
  } else {
    if (j.count(status::error) > 0) {
      return Result{Error{j.at(status::error).get<std::string>()}};
    } else if (j.count(key::msg) > 0) {
      return Result{Error{j.at(key::msg).get<std::string>()}};
    } else if (j.count(key::message) > 0) {
      return Result{Error{j.at(key::message).get<std::string>()}};
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