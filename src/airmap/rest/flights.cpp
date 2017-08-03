#include <airmap/rest/flights.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <boost/format.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

airmap::rest::Flights::Flights(Communicator& communicator) : communicator_{communicator} {
}

void airmap::rest::Flights::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  communicator_.get("api.airmap.com", "/flight/v2", std::move(query), std::move(headers),
                    [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Flight>>(json::parse(result.value())));
                      }
                    });
}

void airmap::rest::Flights::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  communicator_.get("api.airmap.com", (boost::format("/flight/v2/%1%") % parameters.id).str(), std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<Flight>(json::parse(result.value())));
                      }
                    });
}

void airmap::rest::Flights::create_flight_by_point(const CreateFlight::Parameters& parameters,
                                                   const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  communicator_.post("api.airmap.com", "/flight/v2/point", std::move(headers), j.dump(),
                     [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<Flight>(json::parse(result.value())));
                       }
                     });
}

void airmap::rest::Flights::create_flight_by_path(const CreateFlight::Parameters& parameters,
                                                  const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  communicator_.post("api.airmap.com", "/flight/v2/path", std::move(headers), j.dump(),
                     [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<Flight>(json::parse(result.value())));
                       }
                     });
}

void airmap::rest::Flights::create_flight_by_polygon(const CreateFlight::Parameters& parameters,
                                                     const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  communicator_.post("api.airmap.com", "/flight/v2/polygon", std::move(headers), j.dump(),
                     [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<Flight>(json::parse(result.value())));
                       } else {
                         cb(CreateFlight::Result{result.error()});
                       }
                     });
}

void airmap::rest::Flights::end_flight(const EndFlight::Parameters& parameters, const EndFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  communicator_.post("api.airmap.com", (boost::format("/flight/v2/%1%/end") % parameters.id).str(), std::move(headers),
                     std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<EndFlight::Response>(json::parse(result.value())));
                       }
                     });
}

void airmap::rest::Flights::delete_flight(const DeleteFlight::Parameters& parameters,
                                          const DeleteFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  communicator_.post("api.airmap.com", (boost::format("/flight/v2/%1%/delete") % parameters.id).str(),
                     std::move(headers), std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<DeleteFlight::Response>(json::parse(result.value())));
                       }
                     });
}

void airmap::rest::Flights::start_flight_communications(const StartFlightCommunications::Parameters& parameters,
                                                        const StartFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  communicator_.post("api.airmap.com", (boost::format("/flight/v2/%1%/start-comm") % parameters.id).str(),
                     std::move(headers), std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<StartFlightCommunications::Response>(json::parse(result.value())));
                       }
                     });
}

void airmap::rest::Flights::end_flight_communications(const EndFlightCommunications::Parameters& parameters,
                                                      const EndFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  communicator_.post("api.airmap.com", (boost::format("/flight/v2/%1%/end-comm") % parameters.id).str(),
                     std::move(headers), std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<EndFlightCommunications::Response>(json::parse(result.value())));
                       }
                     });
}