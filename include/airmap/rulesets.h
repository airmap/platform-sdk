#ifndef AIRMAP_RULESETS_H_
#define AIRMAP_RULESETS_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/ruleset.h>
#include <airmap/outcome.h>
#include <airmap/geometry.h>

#include <cstdint>
#include <functional>
#include <vector>

namespace airmap {

class RuleSets : DoNotCopyOrMove {
 public:
  struct ForGeometry {
    struct Parameters {
      Geometry geometry;
    };

    using Result = Outcome<std::vector<RuleSet>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct ForId {
    struct Parameters {
      Id id;
    };

    using Result = Outcome<RuleSet, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct GetRules {
    struct Parameters {
      std::string rulesets;
    };

    using Result = Outcome<std::vector<RuleSet>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct EvaluateRules {
    struct Parameters {
      Geometry geometry;
      // TBD - list of features
      Feature flight_features;
      std::string rulesets;
    };

    using Result = Outcome<RuleSet, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  virtual void for_geometry(const ForGeometry::Parameters& parameters, const ForGeometry::Callback& cb) = 0;

  virtual void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) = 0;

  virtual void get_rules(const GetRules::Parameters& parameters, const GetRules::Callback& cb) = 0;

  virtual void evaluate_rules(const EvaluateRules::Parameters& parameters, const EvaluateRules::Callback& cb) = 0;

 protected:
  /// @cond
  RuleSets() = default;
  /// @endcond
};

}  // namespace airmap

#endif  // AIRMAP_RULESETS_H_