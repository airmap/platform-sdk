//
//  status.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_REST_STATUS_H_
#define AIRMAP_REST_STATUS_H_

#include <airmap/client.h>
#include <airmap/net/http/requester.h>
#include <airmap/status.h>

namespace airmap {
namespace rest {

class Status : public airmap::Status {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit Status(const std::shared_ptr<net::http::Requester>& requester);

  void get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;
  void get_status_by_polygon(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) override;

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_STATUS_H_
