#include <airmap/status.h>

#include <iosfwd>

std::ostream& airmap::operator<<(std::ostream& out, Status::Color color) {
  switch (color) {
    case Status::Color::green:
      out << "green";
      break;
    case Status::Color::yellow:
      out << "yellow";
      break;
    case Status::Color::orange:
      out << "orange";
      break;
    case Status::Color::red:
      out << "red";
      break;
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, Status::Color& color) {
  std::string s;
  in >> s;

  if (s == "green")
    color = Status::Color::green;
  else if (s == "yellow")
    color = Status::Color::yellow;
  else if (s == "orange")
    color = Status::Color::orange;
  else if (s == "red")
    color = Status::Color::red;

  return in;
}