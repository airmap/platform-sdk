#ifndef AIRMAP_FLIGHT_PLAN_H_
#define AIRMAP_FLIGHT_PLAN_H_

#include <airmap/date_time.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/pilot.h>

#include <cstdint>

namespace airmap {

/// FlightPlan bundles together properties describing a plan for a flight.
struct FlightPlan {
  using Id = std::string;

  Optional<Id> id;           ///< The unique identifier of a flight in the context of AirMap.
  Pilot pilot;               ///< The pilot responsible for the flight.
  Pilot::Aircraft aircraft;  ///< The aircraft conducting the flight.
  float latitude;            ///< The latitude component of the takeoff point in [°].
  float longitude;           ///< The longitude component of the takeoff point in [°].
  float max_altitude;        ///< The maximum altitude over the entire flight in [m].
  float min_altitude;        ///< The minimum altitude over the entire flight in [m].
  float buffer;              ///< The buffer in [m] around the geometry.
  Geometry geometry;         ///< The geometry describing the flight.
  DateTime start_time;       ///< Point in time when the flight will start/was started.
  DateTime end_time;         ///< Point in time when the fligth will end.
};

}  // namespace airmap

#endif  // AIRMAP_FLIGHT_PLAN_H_
