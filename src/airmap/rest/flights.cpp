#include <airmap/rest/flights.h>

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

airmap::rest::Flights::Flights(const std::string& host, Client::Version version, Communicator& communicator)
    : host_{host}, version_{version}, communicator_{communicator} {
}

void airmap::rest::Flights::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  communicator_.get(host_, version_to_path(version_, "/flight/%s"), std::move(query), std::move(headers),
                    [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<std::vector<Flight>>(json::parse(result.value())));
                      } else {
                        cb(Search::Result{result.error()});
                      }
                    });
}

void airmap::rest::Flights::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  if (parameters.authorization)
    headers["Authorization"] = (boost::format("Bearer %1%") % parameters.authorization.get()).str();
  codec::http::query::encode(query, parameters);

  communicator_.get(host_, version_to_path(version_, "/flight/%s/%s", parameters.id), std::move(query),
                    std::move(headers), [cb](const Communicator::DoResult& result) {
                      if (result) {
                        cb(jsend::to_outcome<Flight>(json::parse(result.value())));
                      } else {
                        cb(ForId::Result{result.error()});
                      }
                    });
}

void airmap::rest::Flights::create_flight_by_point(const CreateFlight::Parameters& parameters,
                                                   const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  communicator_.post(host_, version_to_path(version_, "/flight/%s/point"), std::move(headers), j.dump(),
                     [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<Flight>(json::parse(result.value())));
                       } else {
                         cb(CreateFlight::Result{result.error()});
                       }
                     });
}

void airmap::rest::Flights::create_flight_by_path(const CreateFlight::Parameters& parameters,
                                                  const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  communicator_.post(host_, version_to_path(version_, "/flight/%s/path"), std::move(headers), j.dump(),
                     [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<Flight>(json::parse(result.value())));
                       } else {
                         cb(CreateFlight::Result{result.error()});
                       }
                     });
}

void airmap::rest::Flights::create_flight_by_polygon(const CreateFlight::Parameters& parameters,
                                                     const CreateFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  json j;
  j = parameters;

  communicator_.post(host_, version_to_path(version_, "/flight/%s/polygon"), std::move(headers), j.dump(),
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

  communicator_.post(host_, version_to_path(version_, "/flight/%s/%s/end", parameters.id), std::move(headers),
                     std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<EndFlight::Response>(json::parse(result.value())));
                       } else {
                         cb(EndFlight::Result{result.error()});
                       }
                     });
}

void airmap::rest::Flights::delete_flight(const DeleteFlight::Parameters& parameters,
                                          const DeleteFlight::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  communicator_.post(host_, version_to_path(version_, "/flight/%s/%s/delete", parameters.id), std::move(headers),
                     std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<DeleteFlight::Response>(json::parse(result.value())));
                       } else {
                         cb(DeleteFlight::Result{result.error()});
                       }
                     });
}

void airmap::rest::Flights::start_flight_communications(const StartFlightCommunications::Parameters& parameters,
                                                        const StartFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  communicator_.post(host_, version_to_path(version_, "/flight/%s/%s/start-comm", parameters.id), std::move(headers),
                     std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<StartFlightCommunications::Response>(json::parse(result.value())));
                       } else {
                         cb(StartFlightCommunications::Result{result.error()});
                       }
                     });
}

void airmap::rest::Flights::end_flight_communications(const EndFlightCommunications::Parameters& parameters,
                                                      const EndFlightCommunications::Callback& cb) {
  std::unordered_map<std::string, std::string> headers{
      {"Authorization", (boost::format("Bearer %1%") % parameters.authorization).str()}};

  communicator_.post(host_, version_to_path(version_, "/flight/%s/%s/end-comm", parameters.id), std::move(headers),
                     std::string{}, [cb](const Communicator::DoResult& result) {
                       if (result) {
                         cb(jsend::to_outcome<EndFlightCommunications::Response>(json::parse(result.value())));
                       } else {
                         cb(EndFlightCommunications::Result{result.error()});
                       }
                     });
}
