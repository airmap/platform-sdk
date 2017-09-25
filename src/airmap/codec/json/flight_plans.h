
#ifndef AIRMAP_CODEC_JSON_FLIGHT_PLANS_H_
#define AIRMAP_CODEC_JSON_FLIGHT_PLANS_H_

#include <airmap/flight_plans.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, FlightPlans::Delete::Response& r);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHT_PLANS_H_
