//
//  date_time.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/date_time.h>

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <memory>

namespace airmap {

struct DateTime::Impl {
  boost::posix_time::ptime ptime;
};

DateTime Clock::universal_time() {
  auto impl   = std::unique_ptr<DateTime::Impl>(new DateTime::Impl());
  impl->ptime = boost::posix_time::microsec_clock::universal_time();

  return DateTime(std::move(impl));
}

DateTime Clock::local_time() {
  auto impl   = std::unique_ptr<DateTime::Impl>(new DateTime::Impl());
  impl->ptime = boost::posix_time::microsec_clock::local_time();

  return DateTime(std::move(impl));
}

struct detail::Duration::Impl {
  boost::posix_time::time_duration time_duration;
};

detail::Duration::Duration() : impl(std::unique_ptr<Impl>(new Impl())) {
}

detail::Duration::Duration(detail::Duration const& old)
    : impl(std::unique_ptr<Impl>(new Impl{old.impl->time_duration})) {
}

uint64_t detail::Duration::total_seconds() const {
  return impl->time_duration.total_seconds();
}

uint64_t detail::Duration::total_microseconds() const {
  return impl->time_duration.total_microseconds();
}

uint64_t detail::Duration::total_milliseconds() const {
  return impl->time_duration.total_milliseconds();
}

uint64_t detail::Duration::hours() const {
  return impl->time_duration.hours();
}

detail::Duration::~Duration() = default;

DateTime::DateTime() : impl(std::unique_ptr<Impl>(new Impl())) {
}
DateTime::DateTime(DateTime const& old) : impl(std::unique_ptr<Impl>(new Impl{old.impl->ptime})) {
}
DateTime::DateTime(DateTime&& old) : impl(std::move(old.impl)) {
}
DateTime::DateTime(std::unique_ptr<DateTime::Impl>&& impl) : impl(std::move(impl)) {
}

DateTime::~DateTime() = default;

DateTime& DateTime::operator=(const DateTime& other) {
  if (this != &other) {
    impl = std::unique_ptr<Impl>(new Impl{other.impl->ptime});
  }

  return *this;
}

DateTime& DateTime::operator=(DateTime&& other) {
  impl = std::move(other.impl);
  return *this;
}

DateTime DateTime::operator+(const detail::Duration& other) const {
  DateTime date_time;
  date_time.impl->ptime = this->impl->ptime + other.impl->time_duration;

  return date_time;
}

Microseconds DateTime::operator-(const DateTime& other) const {
  Microseconds time_duration;
  time_duration.impl->time_duration = this->impl->ptime - other.impl->ptime;

  return time_duration;
}

bool DateTime::operator==(const DateTime& other) const {
  return this->impl->ptime == other.impl->ptime;
}

bool DateTime::operator!=(const DateTime& other) const {
  return !(*this == other);
}

std::istream& operator>>(std::istream& from, DateTime& to) {
  from >> to.impl->ptime;
  return from;
}

std::ostream& operator<<(std::ostream& to, const DateTime& from) {
  to << from.impl->ptime;
  return to;
}

DateTime DateTime::date() const {
  DateTime dt;
  dt.impl->ptime = boost::posix_time::ptime(this->impl->ptime.date());

  return dt;
}

namespace boost_iso {

DateTime datetime(const std::string& iso_time) {
  auto impl   = std::unique_ptr<DateTime::Impl>(new DateTime::Impl());
  impl->ptime = boost::posix_time::from_iso_string(iso_time);

  return DateTime(std::move(impl));
}

std::string to_iso_string(const DateTime& datetime) {
  return boost::posix_time::to_iso_string(datetime.impl->ptime);
}

}  // namespace boost_iso

Microseconds DateTime::time_of_day() const {
  Microseconds time_duration;
  time_duration.impl->time_duration = this->impl->ptime.time_of_day();

  return time_duration;
}

Hours hours(int64_t raw) {
  Hours hours;
  hours.impl->time_duration = boost::posix_time::hours(raw);

  return hours;
}

Minutes minutes(int64_t raw) {
  Minutes minutes;
  minutes.impl->time_duration = boost::posix_time::minutes(raw);

  return minutes;
}

Seconds seconds(int64_t raw) {
  Seconds seconds;
  seconds.impl->time_duration = boost::posix_time::seconds(raw);

  return seconds;
}

Milliseconds milliseconds(int64_t raw) {
  Milliseconds milliseconds;
  milliseconds.impl->time_duration = boost::posix_time::milliseconds{raw};

  return milliseconds;
}

Microseconds microseconds(int64_t raw) {
  Microseconds microseconds;
  microseconds.impl->time_duration = boost::posix_time::microseconds{raw};

  return microseconds;
}

}  // namespace airmap

namespace {
constexpr const char* format{"%Y-%m-%dT%H:%M:%sZ"};
const auto epoch(airmap::boost_iso::datetime("19700101T000000,000000000"));
}  // namespace

uint64_t airmap::milliseconds_since_epoch(const DateTime& dt) {
  return (dt - epoch).total_milliseconds();
}

uint64_t airmap::microseconds_since_epoch(const DateTime& dt) {
  return (dt - epoch).total_microseconds();
}

airmap::DateTime airmap::from_seconds_since_epoch(const Seconds& s) {
  return epoch + s;
}

airmap::DateTime airmap::from_milliseconds_since_epoch(const Milliseconds& ms) {
  return epoch + ms;
}

airmap::DateTime airmap::from_microseconds_since_epoch(const Microseconds& us) {
  return epoch + us;
}

airmap::DateTime airmap::move_to_hour(const DateTime& dt, uint64_t hour) {
  return dt + hours(((hour > dt.time_of_day().hours()) ? 0 : 24) + hour - dt.time_of_day().hours());
}

airmap::DateTime airmap::iso8601::parse(const std::string& s) {
  boost::posix_time::time_input_facet facet{format, 1};

  std::istringstream iss{s};
  iss.imbue(std::locale{iss.getloc(), &facet});
  boost::posix_time::ptime result;
  iss >> result;

  const std::string iso_time(boost::posix_time::to_iso_string(result));
  DateTime dt(boost_iso::datetime(iso_time));
  return dt;
}

std::string airmap::iso8601::generate(const DateTime& dt) {
  boost::posix_time::time_facet facet(1);
  facet.format(format);

  std::ostringstream oss;
  oss.imbue(std::locale(oss.getloc(), &facet));

  boost::posix_time::ptime boost_dt = boost::posix_time::from_iso_string(boost_iso::to_iso_string(dt));
  oss << boost_dt;

  return oss.str();
}

std::ostream& airmap::operator<<(std::ostream& to, const airmap::detail::Duration& from) {
  to << from.total_seconds();
  return to;
}
