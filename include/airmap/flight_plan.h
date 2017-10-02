#ifndef AIRMAP_FLIGHT_PLAN_H_
#define AIRMAP_FLIGHT_PLAN_H_

#include <airmap/date_time.h>
#include <airmap/geometry.h>
#include <airmap/pilot.h>
#include <airmap/status.h>

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

/// FlightPlan bundles together properties describing a plan for a flight.
struct FlightPlan {
  using Id = std::string;

  struct Briefing {
    struct RuleSet {
      enum class Type { pickone, required, optional };

      using Id = std::string;

      Id id;
      Type type;
      std::string name;
      std::string short_name;
      std::string summary;
      bool is_default;
      std::vector<std::string> layers;
      std::vector<Rule> rules;
    };

    struct Jurisdiction {
      enum class Region { national, state, county, city, local };
      using Id = std::uint64_t;

      Id id;
      std::string name;
      Region region;
      std::vector<RuleSet> rulesets;
    };

    struct AdvisoryStatus {
      Status::Color color;
      std::vector<Status::Advisory> advisories;
    };

    struct Authority {
      std::string name;
    };

    struct Authorization {
      enum class Status { accepted, rejected, pending, accepted_upon_submission, rejected_upon_submission };

      Status status;
      Authority authority;
      std::string message;
    };

    struct Validation {
      enum class Status { valid, invalid, unknown };

      struct Feature {
        std::string code;
        std::string description;
      };

      Status status;
      std::string data;
      std::string message;
      Feature feature;
      Authority authority;
    };

    DateTime created_at;
    AdvisoryStatus airspace;
    std::vector<RuleSet> rulesets;
    std::vector<Validation> validations;
    std::vector<Authorization> authorizations;
  };

  Id id;                     ///< The unique identifier of a flight in the context of AirMap.
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

std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::RuleSet::Type type);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::RuleSet::Type& type);

std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::Jurisdiction::Region region);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::Jurisdiction::Region& region);

std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::Authorization::Status status);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::Authorization::Status& status);

std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::Validation::Status status);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::Validation::Status& status);

}  // namespace airmap

#endif  // AIRMAP_FLIGHT_PLAN_H_
