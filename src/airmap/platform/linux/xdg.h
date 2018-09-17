// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef AIRMAP_PLATFORM_LINUX_XDG_H_
#define AIRMAP_PLATFORM_LINUX_XDG_H_

#include <boost/filesystem.hpp>

#include <string>
#include <vector>

namespace airmap {
namespace platform {
namespace linux_ {
namespace xdg {
// NotCopyable deletes the copy c'tor and the assignment operator.
struct NotCopyable {
  NotCopyable()                   = default;
  NotCopyable(const NotCopyable&) = delete;
  virtual ~NotCopyable()          = default;
  NotCopyable& operator=(const NotCopyable&) = delete;
};

// NotMoveable deletes the move c'tor and the move assignment operator.
struct NotMoveable {
  NotMoveable()              = default;
  NotMoveable(NotMoveable&&) = delete;
  virtual ~NotMoveable()     = default;
  NotMoveable& operator=(NotMoveable&&) = delete;
};

// Data provides functions to query the XDG_DATA_* entries.
class Data : NotCopyable, NotMoveable {
 public:
  // home returns the base directory relative to which user specific
  // data files should be stored.
  virtual boost::filesystem::path home() const;
  // dirs returns the preference-ordered set of base directories to
  // search for data files in addition to the $XDG_DATA_HOME base
  // directory.
  virtual std::vector<boost::filesystem::path> dirs() const;
};

// Config provides functions to query the XDG_CONFIG_* entries.
class Config : NotCopyable, NotMoveable {
 public:
  // home returns the base directory relative to which user specific
  // configuration files should be stored.
  virtual boost::filesystem::path home() const;
  // dirs returns the preference-ordered set of base directories to
  // search for configuration files in addition to the
  // $XDG_CONFIG_HOME base directory.
  virtual std::vector<boost::filesystem::path> dirs() const;
};

// Cache provides functions to query the XDG_CACHE_HOME entry.
class Cache : NotCopyable, NotMoveable {
 public:
  // home returns the base directory relative to which user specific
  // non-essential data files should be stored.
  virtual boost::filesystem::path home() const;
};

// Runtime provides functions to query the XDG_RUNTIME_DIR entry.
class Runtime : NotCopyable, NotMoveable {
 public:
  // home returns the base directory relative to which user-specific
  // non-essential runtime files and other file objects (such as
  // sockets, named pipes, ...) should be stored.
  virtual boost::filesystem::path dir() const;
};

// A BaseDirSpecification implements the XDG base dir specification:
//   http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
class BaseDirSpecification : NotCopyable, NotMoveable {
 public:
  // create returns an Implementation of BaseDirSpecification.
  static std::shared_ptr<BaseDirSpecification> create();

  // data returns an immutable Data instance.
  virtual const Data& data() const = 0;
  // config returns an immutable Config instance.
  virtual const Config& config() const = 0;
  // cache returns an immutable Cache instance.
  virtual const Cache& cache() const = 0;
  // runtime returns an immutable Runtime instance.
  virtual const Runtime& runtime() const = 0;

 protected:
  BaseDirSpecification() = default;
};

// data returns an immutable reference to a Data instance.
const Data& data();
// config returns an immutable reference to a Config instance.
const Config& config();
// cache returns an immutable reference to a Cache instance.
const Cache& cache();
// runtime returns an immutable reference to a Runtime instance.
const Runtime& runtime();

}  // namespace xdg
}  // namespace linux_
}  // namespace platform
}  // namespace airmap

#endif  // AIRMAP_PLATFORM_LINUX_XDG_H_
