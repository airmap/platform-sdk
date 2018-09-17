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
#include <airmap/platform/linux/xdg.h>

#include <boost/algorithm/string.hpp>

#include <cstdlib>
#include <stdexcept>

namespace fs  = boost::filesystem;
namespace xdg = airmap::platform::linux_::xdg;

namespace {

fs::path throw_if_not_absolute(const fs::path& p) {
  if (p.has_root_directory())
    return p;

  throw std::runtime_error{"Directores MUST be absolute."};
}

namespace env {
std::string get(const std::string& key, const std::string& default_value) {
  if (auto value = std::getenv(key.c_str()))
    return value;
  return default_value;
}

std::string get_or_throw(const std::string& key) {
  if (auto value = std::getenv(key.c_str())) {
    return value;
  }

  throw std::runtime_error{key + " not set in environment"};
}

constexpr const char* xdg_data_home{"XDG_DATA_HOME"};
constexpr const char* xdg_data_dirs{"XDG_DATA_DIRS"};
constexpr const char* xdg_config_home{"XDG_CONFIG_HOME"};
constexpr const char* xdg_config_dirs{"XDG_CONFIG_DIRS"};
constexpr const char* xdg_cache_home{"XDG_CACHE_HOME"};
constexpr const char* xdg_runtime_dir{"XDG_RUNTIME_DIR"};
}  // namespace env

namespace impl {
class BaseDirSpecification : public xdg::BaseDirSpecification {
 public:
  static const BaseDirSpecification& instance() {
    static const BaseDirSpecification spec;
    return spec;
  }

  BaseDirSpecification() {
  }

  const xdg::Data& data() const override {
    return data_;
  }

  const xdg::Config& config() const override {
    return config_;
  }

  const xdg::Cache& cache() const override {
    return cache_;
  }

  const xdg::Runtime& runtime() const override {
    return runtime_;
  }

 private:
  xdg::Data data_;
  xdg::Config config_;
  xdg::Cache cache_;
  xdg::Runtime runtime_;
};
}  // namespace impl
}  // namespace

fs::path xdg::Data::home() const {
  auto v = env::get(env::xdg_data_home, "");
  if (v.empty())
    return throw_if_not_absolute(fs::path{env::get_or_throw("HOME")} / ".local" / "share");

  return throw_if_not_absolute(fs::path(v));
}

std::vector<fs::path> xdg::Data::dirs() const {
  auto v = env::get(env::xdg_data_dirs, "");
  if (v.empty())
    return {fs::path{"/usr/local/share"}, fs::path{"/usr/share"}};

  std::vector<std::string> tokens;
  tokens = boost::split(tokens, v, boost::is_any_of(":"));
  std::vector<fs::path> result;
  for (const auto& token : tokens) {
    result.push_back(throw_if_not_absolute(fs::path(token)));
  }
  return result;
}

fs::path xdg::Config::home() const {
  auto v = env::get(env::xdg_config_home, "");
  if (v.empty())
    return throw_if_not_absolute(fs::path{env::get_or_throw("HOME")} / ".config");

  return throw_if_not_absolute(fs::path(v));
}

std::vector<fs::path> xdg::Config::dirs() const {
  auto v = env::get(env::xdg_config_dirs, "");
  if (v.empty())
    return {fs::path{"/etc/xdg"}};

  std::vector<std::string> tokens;
  tokens = boost::split(tokens, v, boost::is_any_of(":"));
  std::vector<fs::path> result;
  for (const auto& token : tokens) {
    fs::path p(token);
    result.push_back(throw_if_not_absolute(p));
  }
  return result;
}

fs::path xdg::Cache::home() const {
  auto v = env::get(env::xdg_cache_home, "");
  if (v.empty())
    return throw_if_not_absolute(fs::path{env::get_or_throw("HOME")} / ".cache");

  return throw_if_not_absolute(fs::path(v));
}

fs::path xdg::Runtime::dir() const {
  auto v = env::get(env::xdg_config_home, "");
  if (v.empty()) {
    // We do not fall back gracefully and instead throw, dispatching to calling
    // code for handling the case of a safe user-specfic runtime directory missing.
    throw std::runtime_error{"Runtime directory not set"};
  }

  return throw_if_not_absolute(fs::path(v));
}

std::shared_ptr<xdg::BaseDirSpecification> xdg::BaseDirSpecification::create() {
  return std::make_shared<impl::BaseDirSpecification>();
}

const xdg::Data& xdg::data() {
  return impl::BaseDirSpecification::instance().data();
}

const xdg::Config& xdg::config() {
  return impl::BaseDirSpecification::instance().config();
}

const xdg::Cache& xdg::cache() {
  return impl::BaseDirSpecification::instance().cache();
}

const xdg::Runtime& xdg::runtime() {
  return impl::BaseDirSpecification::instance().runtime();
}
