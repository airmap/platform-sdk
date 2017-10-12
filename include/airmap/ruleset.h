#ifndef AIRMAP_RULESET_H_
#define AIRMAP_RULESET_H_

#include <airmap/date_time.h>
#include <airmap/geometry.h>
#include <airmap/pilot.h>
#include <airmap/status.h>

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>
#include <iostream>

namespace airmap {

struct RuleSet {
  /// Rule models the individual result of a Rule evaluation.
  struct Rule {
    /// Status enumerates all known status codes of a rule.
    enum class Status {
      unknown,          ///< The status of the rule is unknown.
      conflicting,      ///< The rule is conflicting.
      not_conflicting,  ///< The rule is not conflicting, all good to go.
      missing_info,     ///< The evaluation requires further information.
      informational     ///< The rule is of informational nature.
    };

    struct FlightFeature {
    	std::string flight_feature;
    	std::string description;
    	std::string input_type;
    	std::string measurement_type;
    	std::string measurement_unit;
    };

    Status status;               ///< The status of the rule.
    std::string short_text;      ///< The human-readable short summary of the rule.
    std::string description;     ///< The human-readable description of the rule.
    std::int32_t display_order;  ///< An indicator for ordering the ruleset.
    std::vector<FlightFeature> flight_features;
  };

  /// SelectionType enumerates all known types for a RuleSet.
  enum class SelectionType {
    pickone,   ///< One rule from the overall set needs to be picked.
    required,  ///< Satisfying the RuleSet is required.
    optional   ///< Satisfying the RuleSet is not required.
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
  };

  /// Id models a unique identifier for a briefing in the context of AirMap.
  using Id = std::string;

  Id id;                   ///< The unique id.
  SelectionType selection_type;               ///< The selection type.
  std::string name;        ///< The human-readable name.
  std::string short_name;  ///< The human-readable short name.
  std::string description;     ///< The human readable description.
  bool is_default;
  Jurisdiction jurisdiction;
  std::vector<std::string> airspace_types;  ///< The layers that a RuleSet instance applies to.
  std::vector<Rule> rules;          ///< The individual rules in the set.

  };

  std::ostream& operator<<(std::ostream& out, RuleSet::Jurisdiction::Region region);
  std::istream& operator>>(std::istream& in, RuleSet::Jurisdiction::Region& region);
  std::ostream& operator<<(std::ostream& out, RuleSet::SelectionType type);
  std::istream& operator>>(std::istream& in, RuleSet::SelectionType& type);
  std::ostream& operator<<(std::ostream& out, RuleSet::Rule::Status status);
  std::istream& operator>>(std::istream& in, RuleSet::Rule::Status& status);

}
#endif  // AIRMAP_RULESET_H_
