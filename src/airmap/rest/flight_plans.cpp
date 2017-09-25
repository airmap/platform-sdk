#include <airmap/rest/flight_plans.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <fmt/printf.h>
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

airmap::rest::FlightPlans::FlightPlans(Client::Version version, const std::shared_ptr<net::http::Requester>& requester)
    : version_{version}, requester_{requester} {
}

void airmap::rest::FlightPlans::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = fmt::sprintf("Bearer %1%", parameters.authorization.get());

  requester_->get(version_to_path(version_, "/flight/%s/plan/%s", parameters.id), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<FlightPlan>(json::parse(result.value().body)));
                    } else {
                      cb(ForId::Result{result.error()});
                    }
                  });
}

void airmap::rest::FlightPlans::create_by_point(const Create::Parameters& parameters, const Create::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  if (parameters.authorization) {
    headers["Authorization"] = fmt::sprintf("Bearer %1%", parameters.authorization.get());
  };

  json j;
  j = parameters.flight_plan;

  requester_->post(version_to_path(version_, "/flight/%s/plan"), std::move(headers), j.dump(),
                   [cb](const net::http::Requester::Result& result) {
                     if (result) {
                       cb(jsend::to_outcome<FlightPlan>(json::parse(result.value().body)));
                     } else {
                       cb(Create::Result{result.error()});
                     }
                   });
}

void airmap::rest::FlightPlans::create_flight_by_polygon(const Create::Parameters& parameters,
                                                         const Create::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  if (parameters.authorization) {
    headers["Authorization"] = fmt::sprintf("Bearer %1%", parameters.authorization.get());
  };

  json j;
  j = parameters.flight_plan;

  requester_->post(version_to_path(version_, "/flight/%s/plan"), std::move(headers), j.dump(),
                   [cb](const net::http::Requester::Result& result) {
                     if (result) {
                       cb(jsend::to_outcome<FlightPlan>(json::parse(result.value().body)));
                     } else {
                       cb(Create::Result{result.error()});
                     }
                   });
}

void airmap::rest::FlightPlans::update(const Update::Parameters& parameters, const Update::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  if (parameters.authorization) {
    headers["Authorization"] = fmt::sprintf("Bearer %1%", parameters.authorization.get());
  };

  json j;
  j = parameters.flight_plan;

  requester_->patch(version_to_path(version_, "/flight/%s/plan/%s", parameters.flight_plan.id.get()),
                    std::move(headers), j.dump(), [cb](const net::http::Requester::Result& result) {
                      if (result) {
                        cb(jsend::to_outcome<FlightPlan>(json::parse(result.value().body)));
                      } else {
                        cb(Create::Result{result.error()});
                      }
                    });
}

void airmap::rest::FlightPlans::delete_(const Delete::Parameters& parameters, const Delete::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = fmt::sprintf("Bearer %1%", parameters.authorization.get());

  requester_->delete_(version_to_path(version_, "/flight/%s/plan/%s", parameters.id), std::move(query),
                      std::move(headers), [cb](const net::http::Requester::Result& result) {
                        if (result) {
                          cb(jsend::to_outcome<Delete::Response>(json::parse(result.value().body)));
                        } else {
                          cb(Delete::Result{result.error()});
                        }
                      });
}

void airmap::rest::FlightPlans::render_briefing(const RenderBriefing::Parameters& parameters,
                                                const RenderBriefing::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = fmt::sprintf("Bearer %1%", parameters.authorization.get());

  requester_->get(version_to_path(version_, "/flight/%s/plan/%s/briefing", parameters.id), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      // cb(jsend::to_outcome<FlightPlan>(json::parse(result.value().body)));
                      std::cout << result.value().body << std::endl;
                    } else {
                      cb(RenderBriefing::Result{result.error()});
                    }
                  });
}

void airmap::rest::FlightPlans::submit(const Submit::Parameters& parameters, const Submit::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  if (parameters.authorization) {
    headers["Authorization"] = fmt::sprintf("Bearer %1%", parameters.authorization.get());
  };

  requester_->post(version_to_path(version_, "/flight/%s/plan/%s/submit", parameters.id), std::move(headers),
                   std::string{}, [cb](const net::http::Requester::Result& result) {
                     if (result) {
                       cb(jsend::to_outcome<FlightPlan>(json::parse(result.value().body)));
                     } else {
                       cb(Create::Result{result.error()});
                     }
                   });
}
