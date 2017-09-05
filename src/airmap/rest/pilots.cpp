#include <airmap/rest/pilots.h>

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

airmap::rest::Pilots::Pilots(Client::Version version, const std::shared_ptr<net::http::Requester>& requester)
    : version_{version}, requester_{requester} {
}

void airmap::rest::Pilots::authenticated(const Authenticated::Parameters& parameters,
                                         const Authenticated::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/pilot/%s/profile"), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<Pilot>(json::parse(result.value().body)));
                    } else {
                      cb(Authenticated::Result{result.error()});
                    }
                  });
}

void airmap::rest::Pilots::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);
  codec::http::query::encode(query, parameters);

  requester_->get(version_to_path(version_, "/pilot/%s/%s", parameters.id), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<Pilot>(json::parse(result.value().body)));
                    } else {
                      cb(ForId::Result{result.error()});
                    }
                  });
}

void airmap::rest::Pilots::update_for_id(const UpdateForId::Parameters& parameters, const UpdateForId::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  json j = parameters;

  requester_->patch(version_to_path(version_, "/pilot/%s/%s", parameters.id), std::move(headers), j.dump(),
                    [cb](const net::http::Requester::Result& result) {
                      if (result) {
                        cb(jsend::to_outcome<Pilot>(json::parse(result.value().body)));
                      } else {
                        cb(UpdateForId::Result{result.error()});
                      }
                    });
}

void airmap::rest::Pilots::start_verify_pilot_phone_for_id(const StartVerifyPilotPhoneForId::Parameters& parameters,
                                                           const StartVerifyPilotPhoneForId::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  requester_->post(version_to_path(version_, "/pilot/%s/%s/phone/send_token", parameters.id), std::move(headers),
                   std::string{}, [cb](const net::http::Requester::Result& result) {
                     if (result) {
                       cb(jsend::to_outcome<StartVerifyPilotPhoneForId::Empty>(json::parse(result.value().body)));
                     } else {
                       cb(StartVerifyPilotPhoneForId::Result{result.error()});
                     }
                   });
}

void airmap::rest::Pilots::finish_verify_pilot_phone_for_id(const FinishVerifyPilotPhoneForId::Parameters& parameters,
                                                            const FinishVerifyPilotPhoneForId::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  json j = parameters;

  requester_->post(version_to_path(version_, "/pilot/%s/%s/phone/send_token", parameters.id), std::move(headers),
                   j.dump(), [cb](const net::http::Requester::Result& result) {
                     if (result) {
                       cb(jsend::to_outcome<FinishVerifyPilotPhoneForId::Empty>(json::parse(result.value().body)));
                     } else {
                       cb(FinishVerifyPilotPhoneForId::Result{result.error()});
                     }
                   });
}

void airmap::rest::Pilots::aircrafts(const Aircrafts::Parameters& parameters, const Aircrafts::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  requester_->get(version_to_path(version_, "/pilot/%s/%s/aircraft", parameters.id), std::move(query),
                  std::move(headers), [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<Pilot::Aircraft>>(json::parse(result.value().body)));
                    } else {
                      cb(Aircrafts::Result{result.error()});
                    }
                  });
}

void airmap::rest::Pilots::add_aircraft(const AddAircraft::Parameters& parameters, const AddAircraft::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  json j = parameters;

  requester_->post(version_to_path(version_, "/pilot/%s/%s/aircraft", parameters.id), std::move(headers), j.dump(),
                   [cb](const net::http::Requester::Result& result) {
                     if (result) {
                       cb(jsend::to_outcome<Pilot::Aircraft>(json::parse(result.value().body)));
                     } else {
                       cb(AddAircraft::Result{result.error()});
                     }
                   });
}

void airmap::rest::Pilots::delete_aircraft(const DeleteAircraft::Parameters& parameters,
                                           const DeleteAircraft::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  requester_->delete_(version_to_path(version_, "/pilot/%s/%s/aircraft/%s", parameters.id, parameters.aircraft_id),
                      std::move(query), std::move(headers), [cb](const net::http::Requester::Result& result) {
                        if (result) {
                          cb(jsend::to_outcome<DeleteAircraft::Empty>(json::parse(result.value().body)));
                        } else {
                          cb(DeleteAircraft::Result{result.error()});
                        }
                      });
}

void airmap::rest::Pilots::update_aircraft(const UpdateAircraft::Parameters& parameters,
                                           const UpdateAircraft::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;
  headers["Authorization"] = fmt::sprintf("Bearer %s", parameters.authorization);

  json j = parameters;

  requester_->patch(version_to_path(version_, "/pilot/%s/%s/aircraft/%s", parameters.id, parameters.aircraft_id),
                    std::move(headers), j.dump(), [cb](const net::http::Requester::Result& result) {
                      if (result) {
                        cb(jsend::to_outcome<UpdateAircraft::Empty>(json::parse(result.value().body)));
                      } else {
                        cb(UpdateAircraft::Result{result.error()});
                      }
                    });
}
