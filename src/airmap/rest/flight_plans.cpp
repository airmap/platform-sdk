#include <airmap/rest/flight_plans.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>

#include <fmt/printf.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string airmap::rest::FlightPlans::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/flight/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v2");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::FlightPlans::FlightPlans(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::FlightPlans::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization.get());

  requester_->get(fmt::sprintf("/plan/%s", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<FlightPlan>(cb));
}

void airmap::rest::FlightPlans::create_by_polygon(const Create::Parameters& parameters, const Create::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  json j = parameters;

  requester_->post("/plan", std::move(headers), j.dump(), net::http::jsend_parsing_request_callback<FlightPlan>(cb));
}

void airmap::rest::FlightPlans::update(const Update::Parameters& parameters, const Update::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  if (parameters.authorization) {
    headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization.get());
  };

  json j;
  j = parameters.flight_plan;

  requester_->patch(fmt::sprintf("/plan/%s", parameters.flight_plan.id), std::move(headers), j.dump(),
                    net::http::jsend_parsing_request_callback<FlightPlan>(cb));
}

void airmap::rest::FlightPlans::delete_(const Delete::Parameters& parameters, const Delete::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization.get());

  requester_->delete_(fmt::sprintf("/plan/%s", parameters.id), std::move(query), std::move(headers),
                      net::http::jsend_parsing_request_callback<Delete::Response>(cb));
}

void airmap::rest::FlightPlans::render_briefing(const RenderBriefing::Parameters& parameters,
                                                const RenderBriefing::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization.get());

  requester_->get(fmt::sprintf("/plan/%s/briefing", parameters.id), std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<FlightPlan::Briefing>(cb));
}

void airmap::rest::FlightPlans::submit(const Submit::Parameters& parameters, const Submit::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  if (parameters.authorization) {
    headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization.get());
  };

  requester_->post(fmt::sprintf("/plan/%s/submit", parameters.id), std::move(headers), std::string{},
                   net::http::jsend_parsing_request_callback<FlightPlan>(cb));
}
