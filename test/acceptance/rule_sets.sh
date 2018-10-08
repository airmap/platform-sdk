#!/bin/sh
set -ex

path=$(cd "$(dirname "$0")" ; pwd -P)

. "${path}/fixture.sh"

geometry_phoenix="${path}/geometries/phoenix.json"

run_all_test_suites() {
    version=$1
    ${AIRMAP_EXECUTABLE} query-rulesets --ruleset-id="usa_part_107" --log-level=debug --version=$version
    ${AIRMAP_EXECUTABLE} query-rulesets --geometry-file="${geometry_phoenix}" --log-level=debug --version=$version
    ${AIRMAP_EXECUTABLE} fetch-rules --rulesets="usa_part_107,usa_sec_91" --log-level=debug --version=$version
    # TODO(tvoss): Enable once parsing of the evaluate response works properly.
    # ${AIRMAP_EXECUTABLE} evaluate-rulesets --rulesets="usa_part_107,usa_sec_91" --geometry-file="${geometry_phoenix}" --version=$version
}

fixture_set_up

run_all_test_suites production
# run_all_test_suites staging
