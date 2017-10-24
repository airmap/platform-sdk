#!/bin/bash

# TODO(tvoss): Reenable this once the laanc-enabled airports have been decided upon.
# For now, we will *not* error out and the test will always pass.
# set -e

readonly config_pattern_staging="{
  \"credentials\": {
    \"api-key\": \"%s\",
    \"oauth\": {
      \"client-id\": \"%s\",
      \"device-id\": \"%s\",
      \"password\": \"%s\",
      \"username\": \"%s\"
    }
  },
  \"host\": \"api.airmap.com\",
  \"sso\": {
    \"host\": \"sso.airmap.io\",
    \"port\": 443
  },
  \"telemetry\": {
    \"host\": \"api-udp-telemetry.stage.airmap.com\",
    \"port\": 16060
  },
  \"traffic\": {
    \"host\": \"mqtt-stage.airmap.io\",
    \"port\": 8883
  },
  \"version\": \"staging\"
}
"

readonly config_pattern_production="{
  \"credentials\": {
    \"api-key\": \"%s\",
    \"oauth\": {
      \"client-id\": \"%s\",
      \"device-id\": \"%s\",
      \"password\": \"%s\",
      \"username\": \"%s\"
    }
  },
  \"host\": \"api.airmap.com\",
  \"sso\": {
    \"host\": \"sso.airmap.io\",
    \"port\": 443
  },
  \"telemetry\": {
    \"host\": \"api.k8s.stage..airmap.com\",
    \"port\": 16060
  },
  \"traffic\": {
    \"host\": \"mqtt-prod.airmap.io\",
    \"port\": 8883
  },
  \"version\": \"production\"
}
"

function set_up {
    if [[ -v CIRCLECI ]]; then
        echo "setting up ci test environment"
        mkdir -p ~/.config/airmap/production || true
        mkdir -p ~/.config/airmap/staging || true

        printf ${config_pattern_production} \
            ${AIRMAP_CIRCLECI_API_KEY} \
            ${AIRMAP_CIRCLECI_CLIENT_ID} \
            ${AIRMAP_CIRCLECI_DEVICE_ID} \
            ${AIRMAP_CIRCLECI_USERNAME} \
            ${AIRMAP_CIRCLECI_PASSWORD} > ~/.config/airmap/production/config.json || true

        printf ${config_pattern_staging} \
            ${AIRMAP_CIRCLECI_API_KEY} \
            ${AIRMAP_CIRCLECI_CLIENT_ID} \
            ${AIRMAP_CIRCLECI_DEVICE_ID} \
            ${AIRMAP_CIRCLECI_USERNAME} \
            ${AIRMAP_CIRCLECI_PASSWORD} > ~/.config/airmap/staging/config.json || true
    fi
}

function run_all_test_suites {
    version=$1
    ${AIRMAP_EXECUTABLE} login --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.arkansas.pinebluff --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.kentucky.florence --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.nevada.reno --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.phoenix.manual --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.phoenix.schwegg --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.phoenix.university --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.phoenix.zoo --version=${version}
    ${AIRMAP_EXECUTABLE} test --test-suite=laanc.wyoming.tetonvillage --version=${version}
}

set_up

run_all_test_suites production
run_all_test_suites staging
