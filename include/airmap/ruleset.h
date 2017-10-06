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

namespace airmap {

struct RuleSet {
  // TBD - define schema for result
  std::string result;

}  // namespace airmap

#endif  // AIRMAP_RULESET_H_