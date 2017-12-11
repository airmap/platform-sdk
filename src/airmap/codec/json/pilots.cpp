#include <airmap/codec/json/pilots.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::UpdateForId::Parameters& parameters) {
  j["first_name"] = parameters.first_name;
  j["last_name"]  = parameters.last_name;
  j["username"]   = parameters.user_name;
  j["phone"]      = parameters.phone;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::FinishVerifyPilotPhoneForId::Parameters& parameters) {
  j["token"] = parameters.token;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::AddAircraft::Parameters& parameters) {
  j["model_id"] = parameters.model_id;
  j["nickname"] = parameters.nick_name;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::DeleteAircraft::Parameters& parameters) {
  j["aircraft_id"] = parameters.aircraft_id;
}

void airmap::codec::json::encode(nlohmann::json& j, const Pilots::UpdateAircraft::Parameters& parameters) {
  j["nick_name"] = parameters.nick_name;
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::StartVerifyPilotPhoneForId::Empty&) {
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::FinishVerifyPilotPhoneForId::Empty&) {
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::DeleteAircraft::Empty&) {
}

void airmap::codec::json::decode(const nlohmann::json&, Pilots::UpdateAircraft::Empty&) {
}