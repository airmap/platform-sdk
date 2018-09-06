//
//  rule.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_RULE_H_
#define AIRMAP_RULE_H_

#include <airmap/visibility.h>

#include <string>

namespace AIRMAP_EXPORT airmap {

struct Rule {
  // TODO(tvoss): Fill in values once schema is known.
  enum class Type {};
  Type type;
  std::string id;
  std::string name;
  std::string description;
  std::string jurisdiction;
  // TODO(tvoss): Add requirements here.
};

bool operator==(const Rule& lhs, const Rule& rhs);

}  // namespace airmap

#endif  // AIRMAP_RULE_H_
