#!/bin/sh

set -e

config_pattern_staging="{
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
    \"host\": \"telemetry.stage.airmap.com\",
    \"port\": 16060
  },
  \"traffic\": {
    \"host\": \"mqtt-stage.airmap.com\",
    \"port\": 8883
  },
  \"version\": \"staging\"
}
"

config_pattern_production="{
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
    \"host\": \"telemetry.stage.airmap.com\",
    \"port\": 16060
  },
  \"traffic\": {
    \"host\": \"mqtt.airmap.com\",
    \"port\": 8883
  },
  \"version\": \"production\"
}
"

fixture_set_up() {
    if test -z "${AIRMAP_EXECUTABLE}"; then
        exit 1
    fi

    if test "${CIRCLECI}" = "true"; then
        echo "setting up ci test environment"
        mkdir -p ~/.config/airmap/production || true
        mkdir -p ~/.config/airmap/staging || true

        printf "${config_pattern_production}" \
            "${AIRMAP_CIRCLECI_API_KEY}" \
            "${AIRMAP_CIRCLECI_CLIENT_ID}" \
            "${AIRMAP_CIRCLECI_DEVICE_ID}" \
            "${AIRMAP_CIRCLECI_PASSWORD}" \
            "${AIRMAP_CIRCLECI_USERNAME}" > ~/.config/airmap/production/config.json || true

        printf "${config_pattern_staging}" \
            "${AIRMAP_CIRCLECI_API_KEY}" \
            "${AIRMAP_CIRCLECI_CLIENT_ID}" \
            "${AIRMAP_CIRCLECI_DEVICE_ID}" \
            "${AIRMAP_CIRCLECI_PASSWORD}" \
            "${AIRMAP_CIRCLECI_USERNAME}" > ~/.config/airmap/staging/config.json || true
    fi

    "${AIRMAP_EXECUTABLE}" login --log-level=debug --version=production
    "${AIRMAP_EXECUTABLE}" login --log-level=debug --version=staging
}

