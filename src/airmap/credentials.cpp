// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <airmap/credentials.h>

std::istream& airmap::operator>>(std::istream& in, Credentials::Type& type) {
  std::string s;
  in >> s;

  if (s == "anonymous")
    type = Credentials::Type::anonymous;
  else if (s == "oauth")
    type = Credentials::Type::oauth;

  return in;
}

std::ostream& airmap::operator<<(std::ostream& out, Credentials::Type type) {
  switch (type) {
    case Credentials::Type::anonymous:
      return out << "anonymous";
    case Credentials::Type::oauth:
      return out << "oauth";
  }

  return out;
}
