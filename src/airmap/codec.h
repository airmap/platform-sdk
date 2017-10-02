#ifndef AIRMAP_CODEC_H_
#define AIRMAP_CODEC_H_

#include <airmap/codec/http/query/aircrafts.h>
#include <airmap/codec/http/query/airspaces.h>
#include <airmap/codec/http/query/flights.h>
#include <airmap/codec/http/query/pilots.h>
#include <airmap/codec/http/query/status.h>

#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/airspace.h>
#include <airmap/codec/json/authenticator.h>
#include <airmap/codec/json/chrono.h>
#include <airmap/codec/json/client.h>
#include <airmap/codec/json/credentials.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/flight.h>
#include <airmap/codec/json/flight_plan.h>
#include <airmap/codec/json/flight_plans.h>
#include <airmap/codec/json/flights.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>
#include <airmap/codec/json/pilot.h>
#include <airmap/codec/json/pilots.h>
#include <airmap/codec/json/rule.h>
#include <airmap/codec/json/status.h>
#include <airmap/codec/json/token.h>
#include <airmap/codec/json/traffic.h>

namespace airmap {

template <typename T>
inline void from_json(const nlohmann::json& j, T& value) {
  codec::json::decode(j, value);
}

template <typename T>
inline void to_json(nlohmann::json& j, const T& value) {
  codec::json::encode(j, value);
}

}  // namespace airmap

#endif  // AIRMAP_CODEC_H_
