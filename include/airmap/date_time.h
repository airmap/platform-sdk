//
//  date_time.h
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_DATE_TIME_H_
#define AIRMAP_DATE_TIME_H_

#include <airmap/airmap_export.h>
#include <memory>
#include <string>

namespace airmap {
class AIRMAP_EXPORT DateTime;
template <typename Tag>
class AIRMAP_EXPORT Duration;

namespace detail {
class AIRMAP_EXPORT Duration;
}  // namespace detail

namespace tag {

struct AIRMAP_EXPORT Hours {};
struct AIRMAP_EXPORT Minutes {};
struct AIRMAP_EXPORT Seconds {};
struct AIRMAP_EXPORT Milliseconds {};
struct AIRMAP_EXPORT Microseconds {};

}  // namespace tag

using Hours        = Duration<tag::Hours>;
using Minutes      = Duration<tag::Minutes>;
using Seconds      = Duration<tag::Seconds>;
using Milliseconds = Duration<tag::Milliseconds>;
using Microseconds = Duration<tag::Microseconds>;

/// Clock marks the reference for time measurements.
class AIRMAP_EXPORT Clock {
 public:
  static DateTime universal_time();
  static DateTime local_time();
};

namespace boost_iso {

DateTime AIRMAP_EXPORT datetime(const std::string &iso_time);
std::string AIRMAP_EXPORT to_iso_string(const DateTime &);

}  // namespace boost_iso

/// DateTime marks a specific point in time, in reference to Clock.
class AIRMAP_EXPORT DateTime {
 public:
  DateTime();
  ~DateTime();
  DateTime(DateTime const &);
  DateTime(DateTime &&);
  DateTime &operator=(const DateTime &);
  DateTime &operator=(DateTime &&);

  DateTime operator+(const detail::Duration &) const;
  Microseconds operator-(const DateTime &) const;
  bool operator==(const DateTime &) const;
  bool operator!=(const DateTime &) const;

  friend std::istream &operator>>(std::istream &, DateTime &);
  friend std::ostream &operator<<(std::ostream &, const DateTime &);

  DateTime date() const;
  Microseconds time_of_day() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl;

  explicit DateTime(std::unique_ptr<Impl> &&);
  friend DateTime Clock::universal_time();
  friend DateTime Clock::local_time();
  friend DateTime boost_iso::datetime(const std::string &iso_time);
  friend std::string boost_iso::to_iso_string(const DateTime &datetime);
};

Hours AIRMAP_EXPORT hours(int64_t raw);
Minutes AIRMAP_EXPORT minutes(int64_t raw);
Seconds AIRMAP_EXPORT seconds(int64_t raw);
Milliseconds AIRMAP_EXPORT milliseconds(int64_t raw);
Microseconds AIRMAP_EXPORT microseconds(int64_t raw);

namespace detail {

class AIRMAP_EXPORT Duration {
 public:
  Duration();
  ~Duration();
  Duration(Duration const &old);
  Duration &operator=(const Duration &);

  uint64_t total_seconds() const;
  uint64_t total_milliseconds() const;
  uint64_t total_microseconds() const;

  uint64_t hours() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl;

  friend DateTime DateTime::operator+(const detail::Duration &) const;
  friend Microseconds DateTime::operator-(const DateTime &) const;
  friend Microseconds DateTime::time_of_day() const;

  friend Hours airmap::hours(int64_t raw);
  friend Minutes airmap::minutes(int64_t raw);
  friend Seconds airmap::seconds(int64_t raw);
  friend Milliseconds airmap::milliseconds(int64_t raw);
  friend Microseconds airmap::microseconds(int64_t raw);
};

}  // namespace detail

template <typename Tag>
class AIRMAP_EXPORT Duration : public detail::Duration {};

/// milliseconds_since_epoch returns the milliseconds that elapsed since the UNIX epoch.
uint64_t AIRMAP_EXPORT milliseconds_since_epoch(const DateTime &dt);
/// microseconds_since_epoch returns the microseconds that elapsed since the UNIX epoch.
uint64_t AIRMAP_EXPORT microseconds_since_epoch(const DateTime &dt);
/// from_seconds_since_epoch returns a DateTime.
DateTime AIRMAP_EXPORT from_seconds_since_epoch(const Seconds &s);
/// from_milliseconds_since_epoch returns a DateTime.
DateTime AIRMAP_EXPORT from_milliseconds_since_epoch(const Milliseconds &ms);
/// from_microseconds_since_epoch returns a DateTime.
DateTime AIRMAP_EXPORT from_microseconds_since_epoch(const Microseconds &us);

// moves the datetime forward to the specified hour
DateTime AIRMAP_EXPORT move_to_hour(const DateTime &dt, uint64_t hour);

namespace iso8601 {

/// parse parses a DateTime instance from the string s in iso8601 format.
DateTime AIRMAP_EXPORT parse(const std::string &s);
/// generate returns a string in iso8601 corresponding to 'dt'.
std::string AIRMAP_EXPORT generate(const DateTime &dt);

}  // namespace iso8601

std::ostream &operator<<(std::ostream &to, const detail::Duration &from);

}  // namespace airmap

#endif  // AIRMAP_DATE_TIME_H_
