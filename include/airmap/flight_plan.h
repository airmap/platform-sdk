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
  /// Id models a unique identifier for a flight plan in the context of AirMap.
  using Id = std::string;

  /// Briefing bundles together information and data for a flight plan.
  /// The target audience is a hypothetical pilot or operator conducting
  /// the flight described in the flight plan.
  struct Briefing {
    /// RuleSet models a set of rules that apply to flight or flight plan.
    struct RuleSet {
      /// Type enumerates all known types for a RuleSet.
      enum class Type {
        pickone,   ///< One rule from the overall set needs to be picked.
        required,  ///< Satisfying the RuleSet is required.
        optional   ///< Satisfying the RuleSet is not required.
      };
      /// Id models a unique identifier for a briefing in the context of AirMap.
      using Id = std::string;

      Id id;                   ///< The unique id.
      Type type;               ///< The type.
      std::string name;        ///< The human-readable name.
      std::string short_name;  ///< The human-readable short name.
      std::string summary;     ///< The human readable summary.
      bool is_default;
      std::vector<std::string> layers;  ///< The layers that a RuleSet instance applies to.
      std::vector<Rule> rules;          ///< The individual rules in the set.
    };

    /// Jurisdiction describes a jurisdiction in a geographical scope.
    struct Jurisdiction {
      /// Region enumerates all known regional scopes of a jurisdiction.
      enum class Region {
        national,  ///< The jurisdiction applies nation-wide.
        state,     ///< The jurisdiction applies to a specific state.
        county,    ///< The jurisdiction applies to a specific county.
        city,      ///< The jurisdiction applies to a specific city.
        local      ///< The jurisdiction only applies locally.
      };
      /// Id models a unique identifier for a jurisdiction in the context of AirMap.
      using Id = std::uint64_t;

      Id id;                          ///< The unique id.
      std::string name;               ///< The human-readable name.
      Region region;                  ///< The regional scope.
      std::vector<RuleSet> rulesets;  /// The RuleSet instances relevant for a jurisdiction.
    };

    /// AdvisoryStatus summarizes the status of all advisories applying to a specific flight plan.
    struct AdvisoryStatus {
      Status::Color color;                       ///< The overall color of the status.
      std::vector<Status::Advisory> advisories;  ///< The collection of relevant advisories.
    };

    /// Authority models an authority capable of authorizing flight plans.
    struct Authority {
      std::string name;  /// The name of the authority.
    };

    /// Authorization bundles up the authorization status of a flight plan.
    struct Authorization {
      /// Status enumerates all known states of an Authorization.
      enum class Status {
        accepted,                  ///< The flight plan is accepted.
        rejected,                  ///< The flight plan is rejected.
        pending,                   ///< The request for authorization is pending a response.
        accepted_upon_submission,  ///< The request will be accepted on submission.
        rejected_upon_submission   ///< The request will be rejected on submission.
      };

      Status status;        ///< The overall status of the request.
      Authority authority;  ///< The authority that handles the request.
      std::string message;  ///< The human-readable message provided by the authority.
    };

    /// Validation bundles up the validation status of a flight plan.
    struct Validation {
      /// Status enumerates all known states of a Validation.
      enum class Status {
        valid,    ///< The validation succeeded.
        invalid,  ///< The validation was rejected.
        unknown   ///< The status is unknown.
      };

      /// Feature describes a specific feature that requires validation.
      struct Feature {
        std::string code;         ///< The code of the feature.
        std::string description;  ///< The description of the feature.
      };

      Status status;        ///< The overall status of the validation.
      std::string data;     ///< The data provided for validation.
      std::string message;  ///< The human-readable message provided by the authority.
      Feature feature;      ///< The specific feature requiring validation.
      Authority authority;  ///< The authority carrying out the validation.
    };

    DateTime created_at;      ///< The timestamp when the briefing was requested and created by the AirMap services.
    AdvisoryStatus airspace;  ///< The summary over all advisories relevant to a specific briefing/flight plan.
    std::vector<RuleSet> rulesets;        ///< All RuleSet instances relevant to a specific briefing/flight plan.
    std::vector<Validation> validations;  ///< All Validation instances relevant to a specific briefing/flight plan.
    std::vector<Authorization>
        authorizations;  ///< All Authorization instances relevant to a specific briefing/flight plan.
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

/// @cond
std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::RuleSet::Type type);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::RuleSet::Type& type);

std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::Jurisdiction::Region region);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::Jurisdiction::Region& region);

std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::Authorization::Status status);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::Authorization::Status& status);

std::ostream& operator<<(std::ostream& out, FlightPlan::Briefing::Validation::Status status);
std::istream& operator>>(std::istream& in, FlightPlan::Briefing::Validation::Status& status);
/// @endcond

}  // namespace airmap

#endif  // AIRMAP_FLIGHT_PLAN_H_
