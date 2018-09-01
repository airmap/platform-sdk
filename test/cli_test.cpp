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