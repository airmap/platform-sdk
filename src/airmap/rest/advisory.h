//
//  advisory.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_REST_ADVISORY_H_
#define AIRMAP_REST_ADVISORY_H_

#include <airmap/advisory.h>
#include <airmap/client.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

class Advisory : public airmap::Advisory {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit Advisory(const std::shared_ptr<net::http::Requester>& requester);

  void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) override;
  void search(const Search::Parameters& parameters, const Search::Callback& cb) override;
  void report_weather(const ReportWeather::Parameters& parameters, const ReportWeather::Callback& cb) override;

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_ADVISORY_H_
