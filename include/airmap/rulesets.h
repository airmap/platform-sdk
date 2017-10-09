#ifndef AIRMAP_RULESETS_H_
#define AIRMAP_RULESETS_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/geometry.h>
#include <airmap/outcome.h>
#include <airmap/ruleset.h>

#include <cstdint>
#include <functional>
#include <vector>

namespace airmap {

/// RuleSets provides functionality for managing contextual airspace.
class RuleSets : DoNotCopyOrMove {
 public:
  struct Search {
    struct Parameters {
      Required<Geometry> geometry;
    };

    using Result   = Outcome<std::vector<RuleSet>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct ForId {
    struct Parameters {
      RuleSet::Id id;
    };

    using Result   = Outcome<RuleSet, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct GetRules {
    struct Parameters {
      Optional<std::string> rulesets;
    };

    using Result   = Outcome<std::vector<RuleSet>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct Evaluation {
    struct Parameters {
      Required<Geometry> geometry;
      // TBD - key value pairs from predicate_feature.code
      Optional<std::string> flight_features;
      Required<std::string> rulesets;
    };

    using Result   = Outcome<RuleSet, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  virtual void search(const Search::Parameters& parameters, const Search::Callback& cb) = 0;

  virtual void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) = 0;

  virtual void get_rules(const GetRules::Parameters& parameters, const GetRules::Callback& cb) = 0;

  virtual void evaluate_rulesets(const Evaluation::Parameters& parameters, const Evaluation::Callback& cb) = 0;

 protected:
  /// @cond
  RuleSets() = default;




  /// @endcond
};

}  // namespace airmap

#endif  // AIRMAP_RULESETS_H_
