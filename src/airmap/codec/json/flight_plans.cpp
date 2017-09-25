#include <airmap/codec/json/flight_plan.h>

#include <airmap/codec.h>
#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>

void airmap::codec::json::decode(const nlohmann::json& j, FlightPlans::Delete::Response& r) {
  get(r.id, j, "id");
}
