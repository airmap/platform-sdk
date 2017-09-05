#include <airmap/rest/status.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <fmt/printf.h>
#include <boost/format.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

template <typename... Args>
std::string version_to_path(airmap::Client::Version version, const char* pattern, Args&&... args) {
  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v2", std::forward<Args>(args)...);
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage", std::forward<Args>(args)...);
  }

  throw std::logic_error{"should not reach here"};
}

}  // namespace

airmap::rest::Status::Status(Client::Version version, const std::shared_ptr<net::http::Requester>& requester)
    : version_{version}, requester_{requester} {
}

void airmap::rest::Status::get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/status/%s/point"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<Report>(json::parse(result.value().body)));
                    } else {
                      cb(GetStatus::Result{result.error()});
                    }
                  });
}

void airmap::rest::Status::get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/status/%s/path"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<Report>(json::parse(result.value().body)));
                    } else {
                      cb(GetStatus::Result{result.error()});
                    }
                  });
}

void airmap::rest::Status::get_status_by_polygon(const GetStatus::Parameters& parameters,
                                                 const GetStatus::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/status/%s/polygon"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<Report>(json::parse(result.value().body)));
                    } else {
                      cb(GetStatus::Result{result.error()});
                    }
                  });
}
