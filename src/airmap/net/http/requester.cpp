/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  requester.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/net/http/requester.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace uuids = boost::uuids;

namespace {
constexpr const char* component{"airmap::Daemon"};
}  // namespace

airmap::net::http::RoutingRequester::RoutingRequester(const std::string& route, const std::shared_ptr<Requester>& next)
    : route_{route}, next_{next} {
}

void airmap::net::http::RoutingRequester::delete_(const std::string& path,
                                                  std::unordered_map<std::string, std::string>&& query,
                                                  std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  next_->delete_(route_ + path, std::move(query), std::move(headers), std::move(cb));
}

void airmap::net::http::RoutingRequester::get(const std::string& path,
                                              std::unordered_map<std::string, std::string>&& query,
                                              std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  next_->get(route_ + path, std::move(query), std::move(headers), std::move(cb));
}

void airmap::net::http::RoutingRequester::patch(const std::string& path,
                                                std::unordered_map<std::string, std::string>&& headers,
                                                const std::string& body, Callback cb) {
  next_->patch(route_ + path, std::move(headers), body, std::move(cb));
}

void airmap::net::http::RoutingRequester::post(const std::string& path,
                                               std::unordered_map<std::string, std::string>&& headers,
                                               const std::string& body, Callback cb) {
  next_->post(route_ + path, std::move(headers), body, std::move(cb));
}

airmap::net::http::LoggingRequester::LoggingRequester(const std::shared_ptr<Logger>& logger,
                                                      const std::shared_ptr<Requester>& next)
    : log_{logger}, next_{next} {
}

void airmap::net::http::LoggingRequester::delete_(const std::string& path,
                                                  std::unordered_map<std::string, std::string>&& query,
                                                  std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  auto uuid                       = uuids::to_string(uuids::random_generator()());
  request_start_timestamps_[uuid] = Clock::universal_time();

  auto r = log_.debug(component);
  r << "starting delete request " << uuid << " for " << path << ":\n";
  r << "query:\n";
  for (const auto& pair : query) {
    r << "  " << pair.first << "=" << pair.second << "\n";
  }
  r << "headers:\n";
  for (const auto& pair : headers) {
    r << "  " << pair.first << "=" << pair.second << "\n";
  }

  next_->delete_(path, std::move(query), std::move(headers), [this, uuid, cb = std::move(cb)](const auto& result) {
    auto it       = request_start_timestamps_.find(uuid);
    auto duration = Clock::universal_time() - it->second;
    request_start_timestamps_.erase(it);

    if (result) {
      auto r = log_.debug(component);
      r << "successfully finished delete request " << uuid << " in " << duration.total_milliseconds() << " [ms]\n"
        << "  version: " << result.value().version << " status: " << result.value().status << "\n";
      if (!result.value().headers.empty()) {
        r << "  headers:\n";
        for (const auto& pair : result.value().headers)
          r << "    " << pair.first << "=" << pair.second << "\n";
      }
      if (!result.value().body.empty())
        r << "  " << result.value().body;
    } else {
      log_.debugf(component, "failed to finish delete request %s in %d [s]", uuid, duration.total_seconds());
    }

    cb(result);
  });
}

void airmap::net::http::LoggingRequester::get(const std::string& path,
                                              std::unordered_map<std::string, std::string>&& query,
                                              std::unordered_map<std::string, std::string>&& headers, Callback cb) {
  auto uuid                       = uuids::to_string(uuids::random_generator()());
  request_start_timestamps_[uuid] = Clock::universal_time();

  auto r = log_.debug(component);
  r << "starting get request " << uuid << " for " << path << ":\n";
  r << "query:\n";
  for (const auto& pair : query) {
    r << "  " << pair.first << "=" << pair.second << "\n";
  }
  r << "headers:\n";
  for (const auto& pair : headers) {
    r << "  " << pair.first << "=" << pair.second << "\n";
  }

  next_->get(path, std::move(query), std::move(headers), [this, uuid, cb = std::move(cb)](const auto& result) {
    auto it       = request_start_timestamps_.find(uuid);
    auto duration = Clock::universal_time() - it->second;
    request_start_timestamps_.erase(it);

    if (result) {
      auto r = log_.debug(component);
      r << "successfully finished get request " << uuid << " in " << duration.total_milliseconds() << " [ms]\n"
        << "  version: " << result.value().version << " status: " << result.value().status << "\n";
      if (!result.value().headers.empty()) {
        r << "  headers:\n";
        for (const auto& pair : result.value().headers)
          r << "    " << pair.first << "=" << pair.second << "\n";
      }
      if (!result.value().body.empty())
        r << "  " << result.value().body;
    } else {
      log_.debugf(component, "failed to finish get request %s in %d [s]", uuid, duration.total_seconds());
    }

    cb(result);
  });
}

void airmap::net::http::LoggingRequester::patch(const std::string& path,
                                                std::unordered_map<std::string, std::string>&& headers,
                                                const std::string& body, Callback cb) {
  auto uuid                       = uuids::to_string(uuids::random_generator()());
  request_start_timestamps_[uuid] = Clock::universal_time();

  auto r = log_.debug(component);
  r << "starting patch request " << uuid << " for " << path << ":\n"
    << "headers:\n";
  for (const auto& pair : headers) {
    r << "  " << pair.first << "=" << pair.second << "\n";
  }
  r << "body:\n"
    << "  " << body;

  next_->patch(path, std::move(headers), std::move(body), [this, uuid, cb = std::move(cb)](const auto& result) {
    auto it       = request_start_timestamps_.find(uuid);
    auto duration = Clock::universal_time() - it->second;
    request_start_timestamps_.erase(it);

    if (result) {
      auto r = log_.debug(component);
      r << "successfully finished patch request " << uuid << " in " << duration.total_milliseconds() << " [ms]\n"
        << "  version: " << result.value().version << " status: " << result.value().status << "\n";
      if (!result.value().headers.empty()) {
        r << "  headers:\n";
        for (const auto& pair : result.value().headers)
          r << "    " << pair.first << "=" << pair.second << "\n";
      }
      if (!result.value().body.empty())
        r << "  " << result.value().body;
    } else {
      log_.debugf(component, "failed to finish patch request %s in %d [s]", uuid, duration.total_seconds());
    }

    cb(result);
  });
}

void airmap::net::http::LoggingRequester::post(const std::string& path,
                                               std::unordered_map<std::string, std::string>&& headers,
                                               const std::string& body, Callback cb) {
  auto uuid                       = uuids::to_string(uuids::random_generator()());
  request_start_timestamps_[uuid] = Clock::universal_time();

  auto r = log_.debug(component);
  r << "starting post request " << uuid << " for " << path << ":\n"
    << "headers:\n";
  for (const auto& pair : headers) {
    r << "  " << pair.first << "=" << pair.second << "\n";
  }
  r << "body:\n"
    << "  " << body;

  next_->post(path, std::move(headers), std::move(body), [this, uuid, cb = std::move(cb)](const auto& result) {
    auto it       = request_start_timestamps_.find(uuid);
    auto duration = Clock::universal_time() - it->second;
    request_start_timestamps_.erase(it);

    if (result) {
      auto r = log_.debug(component);
      r << "successfully finished post request " << uuid << " in " << duration.total_milliseconds() << " [ms]\n"
        << "  version: " << result.value().version << " status: " << result.value().status << "\n";
      if (!result.value().headers.empty()) {
        r << "  headers:\n";
        for (const auto& pair : result.value().headers)
          r << "    " << pair.first << "=" << pair.second << "\n";
      }
      if (!result.value().body.empty())
        r << "  " << result.value().body;
    } else {
      log_.debugf(component, "failed to finish post request %s in %d [s]", uuid, duration.total_seconds());
    }

    cb(result);
  });
}
