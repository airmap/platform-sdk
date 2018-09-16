/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

//
//  datetime_test.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#define BOOST_TEST_MODULE datetime

#include <airmap/date_time.h>

#include <boost/test/included/unit_test.hpp>

using namespace airmap;

static constexpr auto EPOCH_ISO_STRING            = "19700101T000000";
static constexpr auto ARBITRARY_ISO_STRING        = "14920812T012032.123456";
static constexpr auto ARBITRARY_DATE_ISO_STRING   = "14920812T000000";
static constexpr long ARBITRARY_LONG              = 1291;
static constexpr long ARBITRARY_LONG_LESS_THAN_60 = 42;

BOOST_AUTO_TEST_CASE(epoch_is_initialized_correctly) {
  const auto date_time(boost_iso::datetime(EPOCH_ISO_STRING));

  BOOST_CHECK_EQUAL(0, milliseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(datetime_is_converted_from_and_to_string_correctly) {
  const auto expected_iso_string = ARBITRARY_ISO_STRING;
  const auto date_time(boost_iso::datetime(expected_iso_string));

  BOOST_CHECK_EQUAL(expected_iso_string, boost_iso::to_iso_string(date_time));
}

BOOST_AUTO_TEST_CASE(milliseconds_since_epoch_is_correct) {
  const auto expected_milliseconds = 523;
  const auto date_iso_string       = "19700101T000000.523";
  const auto date_time(boost_iso::datetime(date_iso_string));

  BOOST_CHECK_EQUAL(expected_milliseconds, milliseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(microseconds_since_epoch_is_correct) {
  const auto expected_microseconds = 487;
  const auto date_iso_string       = "19700101T000000.000487";
  const auto date_time(boost_iso::datetime(date_iso_string));

  BOOST_CHECK_EQUAL(expected_microseconds, microseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(from_seconds_since_epoch_is_correct) {
  const Seconds duration_seconds(seconds(24));
  const auto date_time           = from_seconds_since_epoch(duration_seconds);
  const auto expected_iso_string = "19700101T000024";

  BOOST_CHECK_EQUAL(expected_iso_string, boost_iso::to_iso_string(date_time));
}

BOOST_AUTO_TEST_CASE(from_milliseconds_since_epoch_is_correct) {
  const Milliseconds duration_milliseconds(milliseconds(844));
  const auto date_time           = from_milliseconds_since_epoch(duration_milliseconds);
  const auto expected_iso_string = "19700101T000000.844000";

  BOOST_CHECK_EQUAL(expected_iso_string, boost_iso::to_iso_string(date_time));
}

BOOST_AUTO_TEST_CASE(from_microseconds_since_epoch_is_correct) {
  const Microseconds duration_microseconds(microseconds(911));
  const auto date_time           = from_microseconds_since_epoch(duration_microseconds);
  const auto expected_iso_string = "19700101T000000.000911";

  BOOST_CHECK_EQUAL(expected_iso_string, boost_iso::to_iso_string(date_time));
}

BOOST_AUTO_TEST_CASE(hours_are_added_correctly) {
  const auto epoch(boost_iso::datetime(EPOCH_ISO_STRING));
  const auto expected_hours        = ARBITRARY_LONG;
  const auto expected_milliseconds = expected_hours * 60 * 60 * 1000;

  const auto date_time = epoch + hours(expected_hours);

  BOOST_CHECK_EQUAL(expected_milliseconds, milliseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(minutes_are_added_correctly) {
  const auto epoch(boost_iso::datetime(EPOCH_ISO_STRING));
  const auto expected_minutes      = ARBITRARY_LONG;
  const auto expected_milliseconds = expected_minutes * 60 * 1000;

  const auto date_time = epoch + minutes(expected_minutes);

  BOOST_CHECK_EQUAL(expected_milliseconds, milliseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(seconds_are_added_correctly) {
  const auto epoch(boost_iso::datetime(EPOCH_ISO_STRING));
  const auto expected_seconds      = ARBITRARY_LONG;
  const auto expected_milliseconds = expected_seconds * 1000;

  const auto date_time = epoch + seconds(expected_seconds);

  BOOST_CHECK_EQUAL(expected_milliseconds, milliseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(milliseconds_are_added_correctly) {
  const auto epoch(boost_iso::datetime(EPOCH_ISO_STRING));
  const auto expected_milliseconds = ARBITRARY_LONG;

  const auto date_time = epoch + milliseconds(expected_milliseconds);

  BOOST_CHECK_EQUAL(expected_milliseconds, milliseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(microseconds_are_added_correctly) {
  const auto epoch(boost_iso::datetime(EPOCH_ISO_STRING));
  const auto expected_microseconds = ARBITRARY_LONG;

  const auto date_time = epoch + microseconds(expected_microseconds);

  BOOST_CHECK_EQUAL(expected_microseconds, microseconds_since_epoch(date_time));
}

BOOST_AUTO_TEST_CASE(total_seconds_is_correct) {
  const auto expected_duration_s = ARBITRARY_LONG;
  const auto first_date(boost_iso::datetime(ARBITRARY_ISO_STRING));
  const auto second_date = first_date + seconds(expected_duration_s);

  const auto time_duration = second_date - first_date;

  BOOST_CHECK_EQUAL(expected_duration_s, time_duration.total_seconds());
}

BOOST_AUTO_TEST_CASE(total_milliseconds_is_correct) {
  const auto expected_duration_ms = ARBITRARY_LONG;
  const auto first_date(boost_iso::datetime(ARBITRARY_ISO_STRING));
  const auto second_date = first_date + milliseconds(expected_duration_ms);

  const auto time_duration = second_date - first_date;

  BOOST_CHECK_EQUAL(expected_duration_ms, time_duration.total_milliseconds());
}

BOOST_AUTO_TEST_CASE(total_microseconds_is_correct) {
  const auto expected_duration_us = ARBITRARY_LONG;
  const auto first_date(boost_iso::datetime(ARBITRARY_ISO_STRING));
  const auto second_date = first_date + microseconds(expected_duration_us);

  const auto time_duration = second_date - first_date;

  BOOST_CHECK_EQUAL(expected_duration_us, time_duration.total_microseconds());
}

BOOST_AUTO_TEST_CASE(hours_is_correct) {
  const auto expected_duration_h = ARBITRARY_LONG_LESS_THAN_60;
  const auto first_date(boost_iso::datetime(ARBITRARY_ISO_STRING));
  const auto second_date = first_date + hours(expected_duration_h) + minutes(ARBITRARY_LONG_LESS_THAN_60) +
                           seconds(ARBITRARY_LONG_LESS_THAN_60) + microseconds(ARBITRARY_LONG_LESS_THAN_60);

  const auto time_duration = second_date - first_date;

  BOOST_CHECK_EQUAL(expected_duration_h, time_duration.hours());
}

BOOST_AUTO_TEST_CASE(date_is_correct) {
  const auto expected_date_iso_string = ARBITRARY_DATE_ISO_STRING;
  const auto date_time = boost_iso::datetime(expected_date_iso_string) + hours(12) + minutes(23) + seconds(1) +
                         milliseconds(1515) + microseconds(1221);
  const auto date_time_date = date_time.date();

  BOOST_CHECK_EQUAL(expected_date_iso_string, boost_iso::to_iso_string(date_time_date));
}

BOOST_AUTO_TEST_CASE(time_of_day_is_correct) {
  const auto epoch(boost_iso::datetime(EPOCH_ISO_STRING));
  const auto further_into_the_day = epoch + hours(21) + minutes(1) + seconds(53) + milliseconds(42) + microseconds(923);
  const auto expected_milliseconds = milliseconds_since_epoch(further_into_the_day);

  const auto time_of_day = further_into_the_day.time_of_day();

  BOOST_CHECK_EQUAL(expected_milliseconds, time_of_day.total_milliseconds());
}

BOOST_AUTO_TEST_CASE(move_to_hour_same_day) {
  const auto date_time(boost_iso::datetime("20030301T100322"));
  const auto expected_iso_string = "20030301T150322";

  const auto moved_date_time = move_to_hour(date_time, 15);

  BOOST_CHECK_EQUAL(expected_iso_string, boost_iso::to_iso_string(moved_date_time));
}

BOOST_AUTO_TEST_CASE(move_to_hour_next_day) {
  const auto date_time(boost_iso::datetime("20030301T100322"));
  const auto expected_iso_string = "20030302T090322";

  const auto moved_date_time = move_to_hour(date_time, 9);

  BOOST_CHECK_EQUAL(expected_iso_string, boost_iso::to_iso_string(moved_date_time));
}

BOOST_AUTO_TEST_CASE(generates_iso8601_and_parses_without_loss) {
  const auto date_time(boost_iso::datetime(ARBITRARY_ISO_STRING));
  const DateTime generated_and_parsed_date_time(iso8601::parse(iso8601::generate(date_time)));

  BOOST_CHECK_EQUAL(boost_iso::to_iso_string(date_time), boost_iso::to_iso_string(generated_and_parsed_date_time));
}

BOOST_AUTO_TEST_CASE(test_istream_operator) {
  const auto expected_iso_string = "19880715T142311.345678";
  std::stringstream ss("1988-Jul-15 14:23:11.345678");
  DateTime date_time;

  ss >> date_time;

  BOOST_CHECK_EQUAL(expected_iso_string, boost_iso::to_iso_string(date_time));
}

BOOST_AUTO_TEST_CASE(test_ostream_operator) {
  const auto expected_output = "1970-Jan-01 00:00:00";
  const auto date_time(boost_iso::datetime(EPOCH_ISO_STRING));

  std::stringstream ss;
  ss << date_time;

  BOOST_CHECK_EQUAL(expected_output, ss.str());
}

BOOST_AUTO_TEST_CASE(test_equality_operator) {
  const auto date_time1(boost_iso::datetime(ARBITRARY_ISO_STRING));
  const auto date_time2(boost_iso::datetime(ARBITRARY_ISO_STRING));

  BOOST_CHECK(date_time1 == date_time2);
}

BOOST_AUTO_TEST_CASE(test_inequality_operator) {
  const auto date_time1(boost_iso::datetime(EPOCH_ISO_STRING));
  const auto date_time2(boost_iso::datetime(ARBITRARY_ISO_STRING));

  BOOST_CHECK(date_time1 != date_time2);
}

BOOST_AUTO_TEST_CASE(test_move_operator_date_time) {
  auto date_time(boost_iso::datetime(ARBITRARY_ISO_STRING));
  DateTime new_date_time;
  new_date_time = std::move(date_time);

  BOOST_CHECK_EQUAL(ARBITRARY_ISO_STRING, boost_iso::to_iso_string(new_date_time));
}

BOOST_AUTO_TEST_CASE(test_move_constructor_date_time) {
  auto date_time(boost_iso::datetime(ARBITRARY_ISO_STRING));
  DateTime new_date_time(std::move(date_time));

  BOOST_CHECK_EQUAL(ARBITRARY_ISO_STRING, boost_iso::to_iso_string(new_date_time));
}
