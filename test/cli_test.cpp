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
//  cli_test.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#define BOOST_TEST_MODULE cli

#include <airmap/util/cli.h>

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(version_is_read_correctly_from_input_stream) {
  std::stringstream result;

  airmap::util::cli::TabWriter tw;
  tw.write("one")
      .write("two")
      .write("three")
      .write("four")
      .new_line()
      .write("1")
      .write("2")
      .write("3")
      .write("4")
      .fill_with('*')
      .separate_columns_with('|')
      .flush(result);

  std::string line;
  BOOST_CHECK(std::getline(result, line));
  BOOST_CHECK("one|two|three|four" == line);
  BOOST_CHECK(std::getline(result, line));
  BOOST_CHECK("1**|2**|3****|4***" == line);
}